#include <iostream>

#include "components/player_board.hpp"
#include "core/component.hpp"

[[noreturn]] void keyboard_input_listener(const std::shared_ptr<console::keyboard::keyboard>& keyboard,
                                          const std::shared_ptr<core::component>& component)
{
    while (true)
    {
        const auto pressed_key = keyboard->get_pressed_key();
        if (component->handle_keyboard_event(pressed_key))
        {
            if (component->should_repaint())
            {
                component->paint();
            }
        }
        std::cout << std::flush;
    }
}

int main()
{
    const auto keyboard = std::make_shared<console::keyboard::keyboard>();
    keyboard->listen_for_input();

    const auto console = std::make_shared<console::console>(std::cout);
    console->set_cursor_display(false);
    console->clear();

    auto player_board = std::make_shared<components::player_board>(0, 0, console);

    const std::shared_ptr<core::component>& current_component = player_board;

    current_component->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &current_component]()
    {
        keyboard_input_listener(keyboard, current_component);
    });

    keyboard_input_thread.join();

    return 0;
}
