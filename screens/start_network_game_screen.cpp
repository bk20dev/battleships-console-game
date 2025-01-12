#include "start_network_game_screen.hpp"

#include <iostream>

#include "../constants/style.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> start_network_game_screen_keyboard_actions = {
        {
            .key_to_press = components::keyboard_actions::ARROWS,
            .action_description = "Move cursor",
            .key_to_press_length = components::keyboard_actions::ARROWS_LENGTH
        },
        {
            .key_to_press = "Enter",
            .action_description = "Select",
        },
        {
            .key_to_press = "ESC",
            .action_description = "Go back",
        },
    };
}

void screens::start_network_game_screen::initialize_components()
{
    port_input_label = std::make_shared<components::label>(0, 0, child_console_view, "Port");
    port_input = std::make_shared<components::input>(
        6, 0, child_console_view, components::input_type::NUMBER, 5, "3000");

    port_input_hint_label = std::make_shared<components::label>(
        0, 2, child_console_view,
        "The port number is a specific number that will allow the second\n"
        "player to connect. Suggested port numbers range from 1024 to 49151.");
    port_input_hint_label->set_style(constants::style::general::hint_style);

    start_game_button = std::make_shared<components::text_button>(
        0, 6, child_console_view, "Start game", []
        {
            // TODO: Implement establishing a connection
        });
    go_back_button = std::make_shared<components::text_button>(
        0, 7, child_console_view, "Go back", [this]
        {
            on_navigate_up();
        });

    set_keyboard_actions(start_network_game_screen_keyboard_actions);
}

void screens::start_network_game_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(port_input);
    tab_indexer.connect_component(start_game_button);
    tab_indexer.connect_component(go_back_button);
}

screens::start_network_game_screen::start_network_game_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_navigate_up)
    : screen(x, y, console, "Start network game"),
      on_navigate_up(on_navigate_up)
{
    initialize_components();
    initialize_tab_indexer();
}

void screens::start_network_game_screen::paint()
{
    port_input_label->paint();
    port_input->paint();
    port_input_hint_label->paint();
    start_game_button->paint();
    go_back_button->paint();

    screen::paint();
}

bool screens::start_network_game_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key == console::keyboard::character::ESCAPE)
    {
        on_navigate_up();
        return true;
    }

    if (const auto& focused_component = tab_indexer.get_focused_component<component>())
    {
        if (handle_keyboard_event_for_child(key, focused_component))
        {
            return true;
        }
    }

    if (!key.is_vertical_arrow())
    {
        return component::handle_keyboard_event(key);
    }

    const core::offset arrow_key_offset = key.get_arrow_offset();
    tab_indexer.focus_next_component(arrow_key_offset.y);

    return true;
}
