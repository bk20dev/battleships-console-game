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

    std::vector placed_battleships{
        models::battleship{.id = 0, .rectangle = {.position = {3, 3}, .size = {1, 4}}},
        models::battleship{.id = 1, .rectangle = {.position = {5, 4}, .size = {3, 1}}},
        models::battleship{.id = 2, .rectangle = {.position = {1, 1}, .size = {1, 2}}},
    };

    std::vector opponent_shots{
        models::bullet{{1, 2}},
        models::bullet{{4, 5}},
        models::bullet{{3, 3}},
        models::bullet{{7, 4}},
    };

    const auto player_board = std::make_shared<components::player_board>(
        0, 0, console, placed_battleships, opponent_shots
    );

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
