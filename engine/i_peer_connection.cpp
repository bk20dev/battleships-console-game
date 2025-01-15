#include "i_peer_connection.hpp"

void engine::i_peer_connection::notify_board_prepared()
{
}

void engine::i_peer_connection::change_turn(bool current_player)
{
}

void engine::i_peer_connection::notify_shot_fired(const core::position& position)
{
}

void engine::i_peer_connection::notify_battleship_part_damaged(const core::position& damaged_battleship_part)
{
}

void engine::i_peer_connection::notify_battleship_destroyed(const models::battleship& destroyed_battleship)
{
}
