#include "game_controller.hpp"

void engine::game_controller::initialize_players()
{
    current_player = player(
        [this](const core::position& damaged_battleship_part)
        {
            peer_notify_battleship_part_damaged(damaged_battleship_part);
        },
        [this](const models::battleship& destroyed_battleship)
        {
            peer_notify_battleship_destroyed(destroyed_battleship);
        }
    );

    opponent_player = player();
}

void engine::game_controller::initialize_peer_connection(std::shared_ptr<i_peer> peer_connection)
{
    this->peer_connection = peer_connection;

    peer_connection->on_opponent_board_prepared = [this]
    {
        handle_opponent_board_prepared();
    };
    peer_connection->on_turn_changed = [this](const bool opponent_player)
    {
        handle_turn_changed(opponent_player);
    };
    peer_connection->on_opponent_shot_received = [this](const core::position& position)
    {
        handle_opponent_shot_received(position);
    };
    peer_connection->on_opponent_battleship_part_damaged = [this](const core::position& damaged_battleship_part)
    {
        handle_opponent_battleship_part_damaged(damaged_battleship_part);
    };
    peer_connection->on_opponent_battleship_destroyed = [this](const models::battleship& destroyed_battleship)
    {
        handle_opponent_battleship_destroyed(destroyed_battleship);
    };
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

void engine::game_controller::peer_notify_board_prepared() const
{
    peer_connection->notify_board_prepared();
}

void engine::game_controller::peer_change_turn(const bool current_player) const
{
    peer_connection->change_turn(current_player);
}

void engine::game_controller::peer_fire_shot(const core::position& position) const
{
    peer_connection->notify_shot_fired(position);
}

void engine::game_controller::peer_notify_battleship_part_damaged(const core::position& damaged_battleship_part) const
{
    peer_connection->notify_battleship_part_damaged(damaged_battleship_part);
}

void engine::game_controller::peer_notify_battleship_destroyed(const models::battleship& destroyed_battleship) const
{
    peer_connection->notify_battleship_destroyed(destroyed_battleship);
}

void engine::game_controller::handle_opponent_board_prepared()
{
    opponent_player.set_board_ready();
    handle_any_board_prepared();
}

void engine::game_controller::handle_turn_changed(const bool opponent_player)
{
    this->current_player.set_currently_plays(!opponent_player);
    this->opponent_player.set_currently_plays(opponent_player);
}

void engine::game_controller::handle_opponent_shot_received(const core::position& position)
{
    const models::bullet bullet{.position = position};
    current_player.shoot_with(bullet);
}

void engine::game_controller::handle_opponent_battleship_part_damaged(const core::position& damaged_battleship_part)
{
    opponent_player.add_damaged_battleship_part(damaged_battleship_part);
}

void engine::game_controller::handle_opponent_battleship_destroyed(const models::battleship& destroyed_battleship)
{
    opponent_player.add_destroyed_battleship(destroyed_battleship);
}

void engine::game_controller::handle_any_board_prepared() const
{
    if (!current_player.get_is_board_ready()) return;
    if (!opponent_player.get_is_board_ready()) return;
    on_all_boards_ready();
}

engine::game_controller::game_controller(const std::shared_ptr<i_peer>& peer_connection)
{
    initialize_players();
    initialize_peer_connection(peer_connection);
}

void engine::game_controller::set_placed_battleships(
    const std::vector<models::battleship>& current_player_placed_battleships)
{
    current_player.set_placed_battleships(current_player_placed_battleships);
}

void engine::game_controller::mark_board_prepared()
{
    current_player.set_board_ready();

    peer_notify_board_prepared();
    if (!opponent_player.get_is_currently_playing())
    {
        change_turn(/* current player */ true, /* force */ true);
    }

    handle_any_board_prepared();
}

void engine::game_controller::change_turn(const bool selected_player, const bool force)
{
    if (!force && !current_player.get_is_currently_playing())
    {
        return;
    }
    current_player.set_currently_plays(selected_player);
    opponent_player.set_currently_plays(!selected_player);
    peer_change_turn(selected_player);
}

bool engine::game_controller::shoot_opponent_board(const core::position& position) const
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
    peer_fire_shot(position);
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
