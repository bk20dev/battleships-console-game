#pragma once

#include <string>

namespace engine
{
    class message_serializer
    {
    public:
        enum message_type : char
        {
            NOTIFY_BOARD_PREPARED = 'p',
            CHANGE_TURN = 't',
            NOTIFY_SHOT_FIRED = 's',
            NOTIFY_BATTLESHIP_PART_DAMAGED = 'm',
            NOTIFY_BATTLESHIP_DESTROYED = 'd',
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
