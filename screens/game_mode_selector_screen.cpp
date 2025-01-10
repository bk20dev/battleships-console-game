#include "game_mode_selector_screen.hpp"

#include <iostream>

#include "../constants/dimension.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> screen_keyboard_actions = {
        {
            .key_to_press = components::keyboard_actions::VERTICAL_ARROWS,
            .action_description = "Move cursor",
            .key_to_press_length = components::keyboard_actions::VERTICAL_ARROWS_LENGTH,
        },
        {
            .key_to_press = "Enter",
            .action_description = "Select",
        }
    };
}

void screens::game_mode_selector_screen::initialize_game_mode_options() const
{
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

    game_mode_options_list->add_component(play_with_computer_option);
    game_mode_options_list->add_component(start_network_game);
    game_mode_options_list->add_component(join_network_game);
}

void screens::game_mode_selector_screen::initialize_components()
{
    screen_label = std::make_shared<components::label>(0, 1, console_view, "Select game mode");

    game_mode_options_list = std::make_shared<components::list<components::text_list_item>>(0, 3, console_view);
    initialize_game_mode_options();

    keyboard_actions_footer = std::make_shared<components::keyboard_actions::footer_component>(
        0, size.height - 1, size.width, console_view);
    keyboard_actions_footer->set_actions(screen_keyboard_actions);
}

screens::game_mode_selector_screen::game_mode_selector_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_play_with_computer_selected,
    const std::function<void()>& on_start_network_game_selected,
    const std::function<void()>& on_join_network_game_selected)
    : component(x, y, constants::dimension::screen_width, constants::dimension::screen_height, console),
      on_play_with_computer_selected(on_play_with_computer_selected),
      on_start_network_game_selected(on_start_network_game_selected),
      on_join_network_game_selected(on_join_network_game_selected)
{
    initialize_components();
}

void screens::game_mode_selector_screen::paint()
{
    screen_label->paint();
    game_mode_options_list->paint();
    keyboard_actions_footer->paint();

    component::paint();
}

bool screens::game_mode_selector_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    return game_mode_options_list->handle_keyboard_event(key);
}
