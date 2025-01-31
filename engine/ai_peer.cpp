#include "ai_peer.hpp"

#include <random>
#include <__random/random_device.h>

#include "board_generator.hpp"

std::vector<models::position> engine::ai_peer::generate_shot_positions(const models::size& board_size)
{
    static std::random_device random_device;
    static std::mt19937 random_number_generator(random_device());

    const int cell_count = board_size.width * board_size.height;
    std::vector<models::position> generated_shot_positions(cell_count);

    std::ranges::generate(generated_shot_positions, [n = 0, board_size]() mutable
    {
        const int x = n % board_size.width;
        const int y = n / board_size.width;

        n += 1;

        return models::position{.x = x, .y = y};
    });

    std::ranges::shuffle(generated_shot_positions, random_number_generator);

    return generated_shot_positions;
}

void engine::ai_peer::shoot_opponent_board()
{
    if (positions_left_to_shoot.empty())
    {
        return;
    }

    const models::position shot_position = positions_left_to_shoot.back();
    positions_left_to_shoot.pop_back();

    const models::bullet bullet_fired{.position = shot_position};

    opponent_player.shoot_with(bullet_fired);
    if (on_opponent_shot_received)
    {
        on_opponent_shot_received(bullet_fired.position);
    }
}

void engine::ai_peer::handle_opponent_player_board_prepared()
{
    ai_player.set_board_ready();
    opponent_player.set_board_ready();

    if (on_opponent_board_prepared)
    {
        on_opponent_board_prepared();
    }
}

void engine::ai_peer::handle_ai_player_turn()
{
    shoot_opponent_board();

    ai_player.set_currently_plays(false);
    opponent_player.set_currently_plays(true);

    if (on_turn_changed)
    {
        on_turn_changed(false);
    }
}

void engine::ai_peer::handle_ai_player_battleship_part_damaged(const models::position& damaged_battleship_part) const
{
    if (on_opponent_battleship_part_damaged)
    {
        on_opponent_battleship_part_damaged(damaged_battleship_part);
    }
}

void engine::ai_peer::handle_ai_player_battleship_destroyed(const models::battleship& destroyed_battleship) const
{
    if (on_opponent_battleship_destroyed)
    {
        on_opponent_battleship_destroyed(destroyed_battleship);
    }
}

void engine::ai_peer::handle_ai_player_game_lost() const
{
    if (on_opponent_player_lost)
    {
        on_opponent_player_lost();
    }
}

void engine::ai_peer::initialize_players()
{
    ai_player = player(
        [this](const models::position& damaged_battleship_part)
        {
            handle_ai_player_battleship_part_damaged(damaged_battleship_part);
        }, [this](const models::battleship& destroyed_battleship)
        {
            handle_ai_player_battleship_destroyed(destroyed_battleship);
        });

    opponent_player = player();

    const auto generated_battleship_placement = generator::random_board(available_battleships);
    ai_player.set_placed_battleships(generated_battleship_placement);

    positions_left_to_shoot = generate_shot_positions(generator::default_board_size);
}

engine::ai_peer::ai_peer()
{
    initialize_players();
}

void engine::ai_peer::notify_board_prepared()
{
    handle_opponent_player_board_prepared();
}

void engine::ai_peer::change_turn(const bool current_player)
{
    if (ai_player.get_is_game_lost())
    {
        return;
    }

    ai_player.set_currently_plays(!current_player);
    opponent_player.set_currently_plays(current_player);

    if (current_player)
    {
        return;
    }

    handle_ai_player_turn();
}

void engine::ai_peer::notify_shot_fired(const models::position& crosshair_position)
{
    const models::bullet fired_bullet{.position = crosshair_position};
    ai_player.shoot_with(fired_bullet);

    if (ai_player.get_is_game_lost())
    {
        handle_ai_player_game_lost();
    }
}

void engine::ai_peer::notify_battleship_part_damaged(const models::position& damaged_battleship_part)
{
    opponent_player.add_damaged_battleship_part(damaged_battleship_part);
}

void engine::ai_peer::notify_battleship_destroyed(const models::battleship& destroyed_battleship)
{
    opponent_player.add_destroyed_battleship(destroyed_battleship);
}
