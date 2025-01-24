#pragma once

#include <string>

#include "../core/dimension.hpp"
#include "../models/battleship.hpp"

namespace engine
{
    class game_object_serializer final
    {
        enum game_object_identifier : char
        {
            BOOLEAN_FLAG = 'b',
            POSITION = 'p',
            BATTLESHIP = 's',
        };

        static constexpr int game_object_identifier_length = 1;

        static constexpr int boolean_flag_component_count = 1;
        static constexpr int position_component_count = 2;
        static constexpr int battleship_component_count = 5;

        [[nodiscard]] static std::string serialize_components(
            const std::initializer_list<int>& components);
        [[nodiscard]] static std::string serialize_game_object(
            game_object_identifier game_object_identifier, const std::initializer_list<int>& components);

        [[nodiscard]] static std::string extract_serialized_game_object_value(
            game_object_identifier game_object_identifier, const std::string& serialized_game_object);

    public:
        [[nodiscard]] static std::string serialize_boolean_flag(bool flag_to_serialize);
        [[nodiscard]] static std::string serialize_position(const core::position& position_to_serialize);
        [[nodiscard]] static std::string serialize_battleship(const models::battleship& battleship_to_serialize);

        [[nodiscard]] static bool deserialize_boolean_flag(const std::string& serialized_game_object);
        [[nodiscard]] static core::position deserialize_position(const std::string& serialized_game_object);
        [[nodiscard]] static models::battleship deserialize_battleship(const std::string& serialized_game_object);
    };
}
