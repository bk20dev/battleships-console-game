#pragma once

#include <string>

namespace engine
{
    class message_serializer
    {
    public:
        enum message_type : char
        {
            NOTIFY_BOARD_PREPARED = 1,
            CHANGE_TURN = 2,
            NOTIFY_SHOT_FIRED = 3,
            NOTIFY_BATTLESHIP_PART_DAMAGED = 4,
            NOTIFY_BATTLESHIP_DESTROYED = 5,
        };

    private:
        static constexpr int message_identifier_length = 1;

        [[nodiscard]] static message_type get_message_type(char message_type_character);

    public:
        [[nodiscard]] static std::string serialize_message(
            message_type message_type, const std::string& message);

        [[nodiscard]] static message_type deserialize_message(
            const std::string& serialized_message, std::string& result_message);
    };
}
