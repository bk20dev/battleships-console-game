#include "message_serializer.hpp"

#include <format>

engine::message_serializer::message_type engine::message_serializer::get_message_type(char message_type_character)
{
    switch (message_type_character)
    {
    case NOTIFY_BOARD_PREPARED:
    case CHANGE_TURN:
    case NOTIFY_SHOT_FIRED:
    case NOTIFY_BATTLESHIP_PART_DAMAGED:
    case NOTIFY_BATTLESHIP_DESTROYED:
    case NOTIFY_PLAYER_LOST:
        return static_cast<message_type>(message_type_character);
    default:
        const std::string error_message = std::format("Unknown message type \"{}\".", message_type_character);
        throw std::invalid_argument(error_message);
    }
}

std::string engine::message_serializer::serialize_message(
    const message_type message_type, const std::string& message)
{
    const char message_type_character = static_cast<char>(message_type);
    return std::format("{}{}\n", message_type_character, message);
}

engine::message_serializer::message_type engine::message_serializer::deserialize_message(
    const std::string& serialized_message, std::string& result_message)
{
    if (serialized_message.size() < message_identifier_length)
    {
        throw std::invalid_argument("Serialized game object does not contain a header.");
    }

    const char message_type_character = serialized_message[0];
    const message_type message_type = get_message_type(message_type_character);

    result_message = serialized_message.substr(message_identifier_length);
    return message_type;
}
