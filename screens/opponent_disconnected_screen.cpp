#include "opponent_disconnected_screen.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> opponent_disconnected_screen_keyboard_actions = {
        {
            .key_to_press = "Enter",
            .action_description = "Select",
        },
    };
}

void screens::opponent_disconnected_screen::handle_exit_to_main_menu_clicked() const
{
    if (on_exit_to_main_menu)
    {
        on_exit_to_main_menu();
    }
}

void screens::opponent_disconnected_screen::initialize_components()
{
    exit_to_main_menu_button = std::make_shared<components::text_button>(
        0, 0, child_console_view, "Exit to main menu", [this]
        {
            handle_exit_to_main_menu_clicked();
        });
    exit_to_main_menu_button->focus();
}

screens::opponent_disconnected_screen::opponent_disconnected_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_exit_to_main_menu)
    : screen(x, y, console, "Opponent disconnected"),
      on_exit_to_main_menu(on_exit_to_main_menu)
{
    initialize_components();
    set_keyboard_actions(opponent_disconnected_screen_keyboard_actions);
}

bool screens::opponent_disconnected_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (handle_keyboard_event_for_child(key, exit_to_main_menu_button))
    {
        return true;
    }
    return screen::handle_keyboard_event(key);
}

void screens::opponent_disconnected_screen::paint()
{
    exit_to_main_menu_button->paint();
    screen::paint();
}
