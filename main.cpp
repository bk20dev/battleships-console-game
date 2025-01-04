#include <iostream>

#include "components/opponent_board.hpp"
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

    std::vector player_bullets{
        // battleship id=0
        models::bullet{{3, 3}},
        models::bullet{{3, 4}},
        models::bullet{{3, 5}},

        // revealed battleship parts
        models::bullet{{7, 3}},

        // misses
        models::bullet{{7, 3}},
        models::bullet{{8, 2}},
    };

    std::vector revealed_battleship_parts = {
        core::position{7, 3}
    };

    std::vector destroyed_battleships{
        models::battleship{.id = 0, .rectangle = {.position = {3, 3}, .size = {1, 3}}},
    };

    const auto opponent_board = std::make_shared<components::opponent_board>(
        0, 0, console, player_bullets, revealed_battleship_parts, destroyed_battleships
    );

    const std::shared_ptr<core::component>& current_component = opponent_board;

    current_component->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &current_component]()
    {
        keyboard_input_listener(keyboard, current_component);
    });

    keyboard_input_thread.join();

    return 0;
}
