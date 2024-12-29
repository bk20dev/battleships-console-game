#include <iostream>

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

    const auto board_designer = std::make_shared<components::board_designer>(
        0, 0, console, [](const battleship& battleship)
        {
            std::cout << "Selected placement" << std::endl;
        }, []
        {
            std::cout << "Cancelled placement" << std::endl;
        });

    board_designer->battleships = {
        battleship{.id = 1, .rectangle = {.position = {3, 3}, .size = {4, 1}}},
        battleship{.id = 2, .rectangle = {.position = {5, 5}, .size = {1, 2}}},
        battleship{.id = 3, .rectangle = {.position = {1, 7}, .size = {3, 1}}},
        battleship{.id = 4, .rectangle = {.position = {9, 9}, .size = {1, 1}}},
    };

    board_designer->set_selected_battleship(battleship{.id = 1, .rectangle = {.size = {.width = 4, .height = 1}}});

    board_designer->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &board_designer]()
    {
        keyboard_input_listener(keyboard, board_designer);
    });

    keyboard_input_thread.join();

    return 0;
}
