#pragma once

#include <queue>
#include <thread>

namespace console::keyboard
{
    struct key
    {
        int character;
        bool is_special;
    };

    enum character
    {
        BACKSPACE = 127,
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
