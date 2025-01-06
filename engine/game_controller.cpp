#include "game_controller.hpp"

void engine::game_controller::initialize_players()
{
    current_player = player(
        [this](const core::position& damaged_battleship_part)
        {
            remote_add_damaged_battleship_part(damaged_battleship_part);
        },
        [this](const models::battleship& destroyed_battleship)
        {
            remote_add_destroyed_battleship(destroyed_battleship);
        }
    );

    opponent_player = player();
}

bool engine::game_controller::is_opponent_bullet_present(const models::bullet& bullet_to_find) const
{
    return std::ranges::any_of(
        opponent_player.get_opponent_bullets(),
        [bullet_to_find](const models::bullet& current_bullet)
        {
            return current_bullet == bullet_to_find;
        });
}

void engine::game_controller::remote_set_currently_plays(const bool current_player)
{
    // TODO: Send via network
}

void engine::game_controller::remote_shoot_opponent_board(const core::position& position)
{
    // TODO: Send via network
}

void engine::game_controller::remote_add_damaged_battleship_part(const core::position& damaged_battleship_part)
{
    // TODO: Send via network
}

void engine::game_controller::remote_add_destroyed_battleship(const models::battleship& destroyed_battleship)
{
    // TODO: Send via network
}

void engine::game_controller::handle_shoot_current_player_board(const core::position& position)
{
    const models::bullet bullet{.position = position};
    current_player.shoot_with(bullet);
}

void engine::game_controller::handle_add_damaged_opponent_battleship_part(const core::position& damaged_battleship_part)
{
    opponent_player.add_damaged_battleship_part(damaged_battleship_part);
}

void engine::game_controller::handle_destroyed_opponent_battleship(const models::battleship& destroyed_battleship)
{
    opponent_player.add_destroyed_battleship(destroyed_battleship);
}

engine::game_controller::game_controller(const std::vector<models::battleship>& current_player_placed_battleships)
{
    initialize_players();
    current_player.set_placed_battleships(current_player_placed_battleships);
}

void engine::game_controller::set_player(const bool selected_player)
{
    if (!current_player.get_is_currently_playing())
    {
        return;
    }
    current_player.set_currently_plays(selected_player);
    opponent_player.set_currently_plays(!selected_player);
    remote_set_currently_plays(selected_player);
}

bool engine::game_controller::shoot_opponent_board(const core::position& position)
{
    if (!current_player.get_is_currently_playing())
    {
        return false;
    }
    const models::bullet bullet{.position = position};
    if (is_opponent_bullet_present(bullet))
    {
        return false;
    }
    remote_shoot_opponent_board(position);
    return true;
}

const engine::player& engine::game_controller::get_current_player() const
{
    return current_player;
}

const engine::player& engine::game_controller::get_opponent_player() const
{
    return opponent_player;
}
