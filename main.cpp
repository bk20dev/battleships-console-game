#include <iostream>

#include "core/component.hpp"

[[noreturn]] void keyboard_input_listener(const std::shared_ptr<console::keyboard::keyboard>& keyboard,
                                          const std::shared_ptr<core::component>& component)
{
    while (true)
    {
        const auto pressed_key = keyboard->get_pressed_key();
        component->handle_keyboard_event(pressed_key);
    }
}

int main()
{
    const auto keyboard = std::make_shared<console::keyboard::keyboard>();
    keyboard->listen_for_input();

    const auto console_view = std::make_shared<console::console_view>();

    const auto player = std::make_shared<core::container_component>(0, 0, 3, 3, console_view);
    std::thread keyboard_input_thread([&keyboard, &player]()
    {
        keyboard_input_listener(keyboard, player);
    });

    keyboard_input_thread.join();

    return 0;
}
