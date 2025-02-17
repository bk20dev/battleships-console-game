#pragma once

#include <queue>
#include <thread>

#include "../models/battleship.hpp"

namespace console::keyboard
{
    enum character
    {
        ENTER = 10,
        ESCAPE = 27,
        SPACE = 32,
        ARROW_UP = 65,
        ARROW_DOWN = 66,
        ARROW_RIGHT = 67,
        ARROW_LEFT = 68,
        P = 80,
        R = 82,
        BACKSPACE = 127,
    };

    struct key
    {
        int character;
        bool is_special;

        bool operator==(const enum character& character) const;

        [[nodiscard]] bool is_vertical_arrow() const;

        [[nodiscard]] bool is_horizontal_arrow() const;

        [[nodiscard]] bool is_arrow() const;

        [[nodiscard]] models::offset get_arrow_offset() const;
    };

    class keyboard
    {
        std::queue<key> keyboard_input_queue{};
        std::mutex keyboard_input_queue_mutex{};
        std::condition_variable keyboard_input_condition_variable{};

        std::optional<std::thread> keyboard_input_listener_thread = std::nullopt;

        [[noreturn]] void keyboard_input_listener();

    public:
        keyboard();

        key get_pressed_key();

        void listen_for_input();
    };
}
