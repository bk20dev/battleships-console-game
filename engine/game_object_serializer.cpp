#include "game_object_serializer.hpp"

#include <vector>

std::string engine::game_object_serializer::serialize_components(const std::initializer_list<int>& components)
{
    const int component_count = components.size();
    std::vector<char> serialized_components(component_count);

    for (int current_component_index = 0; const int component : components)
    {
        if (component < 0 || component > std::numeric_limits<char>::max())
        {
            throw std::out_of_range("Component value exceeds the maximum value for char.");
        }

        serialized_components[current_component_index] = static_cast<char>(component);
        current_component_index++;
    }

    return std::string(serialized_components.begin(), serialized_components.end());
}

std::string engine::game_object_serializer::serialize_game_object(
    const game_object_identifier game_object_identifier, const std::initializer_list<int>& components)
{
    const std::string serialized_components = serialize_components(components);
    return static_cast<char>(game_object_identifier) + serialized_components;
}

std::string engine::game_object_serializer::extract_serialized_game_object_value(
    const game_object_identifier game_object_identifier, const std::string& serialized_game_object)
{
    if (serialized_game_object.size() < game_object_identifier_length)
    {
        throw std::invalid_argument("Serialized game object does not contain a header.");
    }

    const char expected_game_object_identifier_character = static_cast<char>(game_object_identifier);

    if (serialized_game_object[0] != expected_game_object_identifier_character)
    {
        throw std::invalid_argument("Mismatch between expected and serialized game object identifiers.");
    }

    return serialized_game_object.substr(game_object_identifier_length);
}

std::string engine::game_object_serializer::serialize_boolean_flag(bool flag_to_serialize)
{
    return serialize_game_object(BOOLEAN_FLAG, {flag_to_serialize});
}

std::string engine::game_object_serializer::serialize_position(const core::position& position_to_serialize)
{
    const auto [x, y] = position_to_serialize;
    return serialize_game_object(POSITION, {x, y});
}

std::string engine::game_object_serializer::serialize_battleship(
    const models::battleship& battleship_to_serialize)
{
    const auto [id, rectangle] = battleship_to_serialize;
    const auto [x, y] = rectangle.position;
    const auto [width, height] = rectangle.size;

    return serialize_game_object(BATTLESHIP, {id, x, y, width, height});
}

bool engine::game_object_serializer::deserialize_boolean_flag(const std::string& serialized_game_object)
{
    const std::string serialized_boolean_flag_value =
        extract_serialized_game_object_value(BOOLEAN_FLAG, serialized_game_object);

    if (serialized_boolean_flag_value.length() < boolean_flag_component_count)
    {
        throw std::invalid_argument("Serialized game object is too short.");
    }

    return serialized_boolean_flag_value[0];
}

core::position engine::game_object_serializer::deserialize_position(const std::string& serialized_game_object)
{
    const std::string serialized_position_value =
        extract_serialized_game_object_value(POSITION, serialized_game_object);

    if (serialized_position_value.length() < position_component_count)
    {
        throw std::invalid_argument("Serialized game object is too short.");
    }

    return core::position{
        .x = serialized_position_value[0],
        .y = serialized_position_value[1],
    };
}

models::battleship engine::game_object_serializer::deserialize_battleship(const std::string& serialized_game_object)
{
    const std::string serialized_battleship_value =
        extract_serialized_game_object_value(BATTLESHIP, serialized_game_object);

    if (serialized_battleship_value.length() < battleship_component_count)
    {
        throw std::invalid_argument("Serialized game object is too short.");
    }

    return models::battleship{
        .id = serialized_battleship_value[0],
        .rectangle = {
            .position = {
                .x = serialized_battleship_value[1],
                .y = serialized_battleship_value[2],
            },
            .size = {
                .width = serialized_battleship_value[3],
                .height = serialized_battleship_value[4],
            },
        },
    };
}
