#include <iostream>

#include "core/component.hpp"
#include "screens/board_designer_screen.hpp"

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

    auto battleship_selector_screen = std::make_shared<screens::board_designer_screen>(0, 0, console);

    battleship_selector_screen->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &battleship_selector_screen]()
    {
        keyboard_input_listener(keyboard, battleship_selector_screen);
    });

    keyboard_input_thread.join();

    return 0;
}
