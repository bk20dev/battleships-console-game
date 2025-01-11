#include "game_mode_selector_screen.hpp"

#include <iostream>

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> game_mode_screen_keyboard_actions = {
        {
            .key_to_press = components::keyboard_actions::VERTICAL_ARROWS,
            .action_description = "Move cursor",
            .key_to_press_length = components::keyboard_actions::VERTICAL_ARROWS_LENGTH,
        },
        {
            .key_to_press = "Enter",
            .action_description = "Select",
        },
    };
}

void screens::game_mode_selector_screen::initialize_game_mode_list()
{
    game_mode_list = std::make_shared<components::list<components::text_list_item>>(0, 0, child_console_view);

    const auto play_with_computer_option = std::make_shared<components::text_list_item>(
        0, 0, nullptr, "Play against computer", [this]
        {
            if (on_play_with_computer_selected)
            {
                on_play_with_computer_selected();
            }
        });
    const auto start_network_game = std::make_shared<components::text_list_item>(
        0, 1, nullptr, "Start network game", [this]
        {
            if (on_start_network_game_selected)
            {
                on_start_network_game_selected();
            }
        });
    const auto join_network_game = std::make_shared<components::text_list_item>(
        0, 2, nullptr, "Join network game", [this]
        {
            if (on_join_network_game_selected)
            {
                on_join_network_game_selected();
            }
        });

    game_mode_list->add_component(play_with_computer_option);
    game_mode_list->add_component(start_network_game);
    game_mode_list->add_component(join_network_game);
}

void screens::game_mode_selector_screen::initialize_components()
{
    initialize_game_mode_list();
    set_keyboard_actions(game_mode_screen_keyboard_actions);
}

screens::game_mode_selector_screen::game_mode_selector_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_play_with_computer_selected,
    const std::function<void()>& on_start_network_game_selected,
    const std::function<void()>& on_join_network_game_selected)
    : screen(0, 0, console, "Select game mode"),
      on_play_with_computer_selected(on_play_with_computer_selected),
      on_start_network_game_selected(on_start_network_game_selected),
      on_join_network_game_selected(on_join_network_game_selected)
{
    initialize_components();
}

void screens::game_mode_selector_screen::paint()
{
    game_mode_list->paint();
    screen::paint();
}

bool screens::game_mode_selector_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    return game_mode_list->handle_keyboard_event(key);
}
