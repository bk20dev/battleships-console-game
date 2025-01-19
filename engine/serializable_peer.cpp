#include "serializable_peer.hpp"

void engine::serializable_peer::send_message(const message_serializer::message_type message_type,
                                             const std::string& serialized_game_object) const
{
    const std::string serialized_message = message_serializer.serialize_message(message_type, serialized_game_object);
    connection->send(serialized_message);
}

void engine::serializable_peer::handle_message(const std::string& serialized_message) const
{
    std::string serialized_game_object;
    const message_serializer::message_type message_type = message_serializer
        .deserialize_message(serialized_message, serialized_game_object);

    switch (message_type)
    {
    case message_serializer::NOTIFY_BOARD_PREPARED:
        handle_opponent_board_prepared();
        break;
    case message_serializer::CHANGE_TURN:
        handle_turn_changed(serialized_game_object);
        break;
    case message_serializer::NOTIFY_SHOT_FIRED:
        handle_opponent_shot_received(serialized_game_object);
        break;
    case message_serializer::NOTIFY_BATTLESHIP_PART_DAMAGED:
        handle_opponent_battleship_part_damaged(serialized_game_object);
        break;
    case message_serializer::NOTIFY_BATTLESHIP_DESTROYED:
        handle_opponent_battleship_destroyed(serialized_game_object);
        break;
    }
}

void engine::serializable_peer::handle_opponent_board_prepared() const
{
    if (on_opponent_board_prepared)
    {
        on_opponent_board_prepared();
    }
}

void engine::serializable_peer::handle_turn_changed(const std::string& serialized_game_object) const
{
    const bool opponent_player = game_object_serializer
        .deserialize_boolean_flag(serialized_game_object);

    if (on_turn_changed)
    {
        on_turn_changed(opponent_player);
    }
}

void engine::serializable_peer::handle_opponent_shot_received(const std::string& serialized_game_object) const
{
    const core::position received_shot = game_object_serializer
        .deserialize_position(serialized_game_object);

    if (on_opponent_shot_received)
    {
        on_opponent_shot_received(received_shot);
    }
}

void engine::serializable_peer::handle_opponent_battleship_part_damaged(const std::string& serialized_game_object) const
{
    const core::position damaged_battleship_part = game_object_serializer
        .deserialize_position(serialized_game_object);

    if (on_opponent_battleship_part_damaged)
    {
        on_opponent_battleship_part_damaged(damaged_battleship_part);
    }
}

void engine::serializable_peer::handle_opponent_battleship_destroyed(const std::string& serialized_game_object) const
{
    const models::battleship destroyed_battleship = game_object_serializer
        .deserialize_battleship(serialized_game_object);

    if (on_opponent_battleship_destroyed)
    {
        on_opponent_battleship_destroyed(destroyed_battleship);
    }
}

engine::serializable_peer::serializable_peer(const std::shared_ptr<i_connection> connection)
{
    this->connection = connection;
    this->connection->on_message = [this](const std::string& serialized_message)
    {
        handle_message(serialized_message);
    };
}

void engine::serializable_peer::notify_board_prepared()
{
    send_message(message_serializer::NOTIFY_BOARD_PREPARED);
}

void engine::serializable_peer::change_turn(bool current_player)
{
    const std::string serialized_current_player = game_object_serializer
        .serialize_boolean_flag(current_player);

    send_message(message_serializer::CHANGE_TURN, serialized_current_player);
}

void engine::serializable_peer::notify_shot_fired(const core::position& shot_position)
{
    const std::string serialized_shot_position = game_object_serializer
        .serialize_position(shot_position);

    send_message(message_serializer::NOTIFY_SHOT_FIRED, serialized_shot_position);
}

void engine::serializable_peer::notify_battleship_part_damaged(const core::position& damaged_battleship_part)
{
    const std::string serialized_damaged_battleship_part = game_object_serializer
        .serialize_position(damaged_battleship_part);

    send_message(message_serializer::NOTIFY_BATTLESHIP_PART_DAMAGED, serialized_damaged_battleship_part);
}

void engine::serializable_peer::notify_battleship_destroyed(const models::battleship& destroyed_battleship)
{
    const std::string serialized_destroyed_battleship = game_object_serializer
        .serialize_battleship(destroyed_battleship);

    send_message(message_serializer::NOTIFY_BATTLESHIP_DESTROYED, serialized_destroyed_battleship);
}
