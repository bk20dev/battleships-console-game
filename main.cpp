#include <iostream>
#include <__ranges/all.h>

#include "components/battleship_selector.hpp"
#include "components/board_designer.hpp"
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

    const std::vector all_battleships = {
        battleship{.id = 0, .rectangle = {.size = {.height = 4, .width = 1}}},
        battleship{.id = 1, .rectangle = {.size = {.height = 1, .width = 3}}},
        battleship{.id = 2, .rectangle = {.size = {.height = 3, .width = 1}}},
        battleship{.id = 3, .rectangle = {.size = {.height = 1, .width = 2}}},
        battleship{.id = 4, .rectangle = {.size = {.height = 1, .width = 2}}},
        battleship{.id = 5, .rectangle = {.size = {.height = 1, .width = 2}}},
        battleship{.id = 6, .rectangle = {.size = {.height = 1, .width = 1}}},
        battleship{.id = 7, .rectangle = {.size = {.height = 1, .width = 1}}},
        battleship{.id = 8, .rectangle = {.size = {.height = 1, .width = 1}}},
        battleship{.id = 9, .rectangle = {.size = {.height = 1, .width = 1}}},
    };

    const auto battleship_selector = std::make_shared<components::battleship_selector>(
        0, 0, console, all_battleships, [](const battleship& battleship)
        {
            std::cout << std::format("Selected #{}", battleship.id) << std::endl;
        });

    battleship_selector->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &battleship_selector]()
    {
        keyboard_input_listener(keyboard, battleship_selector);
    });

    keyboard_input_thread.join();


    return 0;
}
