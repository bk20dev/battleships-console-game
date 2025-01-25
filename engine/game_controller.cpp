#include "game_controller.hpp"

#include <iostream>

void engine::game_controller::change_turn(const bool current_player_turn)
{
    opponent_peer_connection->change_turn(current_player_turn);
    handle_turn_changed(current_player_turn);
}

void engine::game_controller::handle_all_battleship_placements_submitted() const
{
    if (on_all_battleship_placements_submitted)
    {
        on_all_battleship_placements_submitted();
    }
}

void engine::game_controller::handle_any_battleship_placement_submitted() const
{
    if (!current_player.get_is_board_ready()) return;
    if (!opponent_player.get_is_board_ready()) return;
    handle_all_battleship_placements_submitted();
}

void engine::game_controller::handle_opponent_player_disconnected() const
{
    if (on_opponent_disconnected)
    {
        on_opponent_disconnected();
    }
}

void engine::game_controller::handle_opponent_player_battleship_placement_submitted()
{
    opponent_player.set_board_ready();

    if (on_opponent_battleship_placement_submitted)
    {
        on_opponent_battleship_placement_submitted();
    }

    handle_any_battleship_placement_submitted();
}

void engine::game_controller::handle_turn_changed(const bool current_player_turn)
{
    current_player.set_currently_plays(current_player_turn);
    opponent_player.set_currently_plays(!current_player_turn);

    if (on_turn_changed)
    {
        on_turn_changed(current_player_turn);
    }
}

void engine::game_controller::handle_opponent_player_shot_received(const core::position& crosshair_position)
{
    const models::bullet opponent_bullet{crosshair_position};
    if (current_player.shoot_with(opponent_bullet) && on_current_player_board_updated)
    {
        on_current_player_board_updated();
    }
}

void engine::game_controller::handle_current_player_battleship_part_damaged(
    const core::position& damaged_part_position) const
{
    opponent_peer_connection->notify_battleship_part_damaged(damaged_part_position);
}

void engine::game_controller::handle_current_player_battleship_destroyed(
    const models::battleship& destroyed_battleship) const
{
    opponent_peer_connection->notify_battleship_destroyed(destroyed_battleship);
}

void engine::game_controller::handle_opponent_player_battleship_part_damaged(
    const core::position& damaged_part_position)
{
    opponent_player.add_damaged_battleship_part(damaged_part_position);
    if (on_opponent_board_updated)
    {
        on_opponent_board_updated();
    }
}

void engine::game_controller::handle_opponent_player_battleship_destroyed(
    const models::battleship& destroyed_battleship)
{
    opponent_player.add_destroyed_battleship(destroyed_battleship);
    if (on_opponent_board_updated)
    {
        on_opponent_board_updated();
    }
}

void engine::game_controller::initialize_players()
{
    current_player = player(
        [this](const core::position& damaged_part_position)
        {
            handle_current_player_battleship_part_damaged(damaged_part_position);
        },
        [this](const models::battleship& destroyed_battleship)
        {
            handle_current_player_battleship_destroyed(destroyed_battleship);
        }
    );

    opponent_player = player();
}

void engine::game_controller::setup_opponent_peer_connection(const std::shared_ptr<i_peer>& opponent_peer_connection)
{
    this->opponent_peer_connection = opponent_peer_connection;

    opponent_peer_connection->on_disconnect = [this]
    {
        handle_opponent_player_disconnected();
    };
    opponent_peer_connection->on_opponent_board_prepared = [this]
    {
        handle_opponent_player_battleship_placement_submitted();
    };
    opponent_peer_connection->on_turn_changed = [this](const bool opponent_player)
    {
        handle_turn_changed(!opponent_player);
    };
    opponent_peer_connection->on_opponent_shot_received = [this](const core::position& bullet_position)
    {
        handle_opponent_player_shot_received(bullet_position);
    };
    opponent_peer_connection->on_opponent_battleship_part_damaged = [this](const core::position& damaged_part_position)
    {
        handle_opponent_player_battleship_part_damaged(damaged_part_position);
    };
    opponent_peer_connection->on_opponent_battleship_destroyed = [this](const models::battleship& destroyed_battleship)
    {
        handle_opponent_player_battleship_destroyed(destroyed_battleship);
    };
}

engine::game_controller::game_controller(const std::shared_ptr<i_peer>& opponent_peer_connection)
{
    initialize_players();
    setup_opponent_peer_connection(opponent_peer_connection);
}

void engine::game_controller::submit_current_player_battleship_placement(
    const std::vector<models::battleship>& battleship_placement)
{
    current_player.set_placed_battleships(battleship_placement);
    current_player.set_board_ready();

    opponent_peer_connection->notify_board_prepared();
    if (!opponent_player.get_is_currently_playing())
    {
        change_turn(/* current_player_turn */ true);
    }

    handle_any_battleship_placement_submitted();
}

void engine::game_controller::shoot_opponent_player(const core::position& crosshair_position)
{
    if (!is_current_player_turn())
    {
        return;
    }

    const models::bullet fired_bullet{.position = crosshair_position};
    if (!opponent_player.shoot_with(fired_bullet))
    {
        return;
    }

    opponent_peer_connection->notify_shot_fired(fired_bullet.position);
    change_turn(/* current_player_turn */ false);
}

bool engine::game_controller::is_opponent_battleship_placement_submitted() const
{
    return opponent_player.get_is_board_ready();
}

bool engine::game_controller::is_current_player_turn() const
{
    return current_player.get_is_currently_playing();
}

const engine::player& engine::game_controller::get_current_player() const
{
    return current_player;
}

const engine::player& engine::game_controller::get_opponent_player() const
{
    return opponent_player;
}
