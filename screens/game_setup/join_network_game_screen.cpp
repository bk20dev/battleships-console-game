#include "join_network_game_screen.hpp"

#include "../../components/button.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> join_network_game_screen_keyboard_actions = {
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

void screens::join_network_game_screen::initialize_components()
{
    ip_input_label = std::make_shared<components::label>(0, 0, child_console_view, "IP Address");
    ip_input = std::make_shared<components::input>(11, 0, child_console_view, components::TEXT, 15, "192.168.123.123");
    port_input_label = std::make_shared<components::label>(0, 1, child_console_view, "Port");
    port_input = std::make_shared<components::input>(5, 1, child_console_view, components::NUMBER, 5, "3000");
    join_game_button = std::make_shared<components::text_button>(0, 4, child_console_view, "Join game", [this]
    {
        handle_join_game_button_clicked();
    });
    go_back_button = std::make_shared<components::text_button>(0, 5, child_console_view, "Go back", [this]
    {
        on_navigate_up();
    });
    set_keyboard_actions(join_network_game_screen_keyboard_actions);
}

void screens::join_network_game_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(ip_input);
    tab_indexer.connect_component(port_input);
    tab_indexer.connect_component(join_game_button);
    tab_indexer.connect_component(go_back_button);
}

void screens::join_network_game_screen::handle_join_game_button_clicked() const
{
    // TODO: Implement establishing a connection

    const auto created_peer = std::make_shared<engine::i_peer>();
    on_peer_created(created_peer);
}

screens::join_network_game_screen::join_network_game_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void(const std::shared_ptr<engine::i_peer>&)>& on_peer_created,
    const std::function<void()>& on_navigate_up)
    : screen(x, y, console, "Join network game"),
      on_navigate_up(on_navigate_up), on_peer_created(on_peer_created)
{
    initialize_components();
    initialize_tab_indexer();
}

void screens::join_network_game_screen::paint()
{
    ip_input_label->paint();
    ip_input->paint();
    port_input_label->paint();
    port_input->paint();
    join_game_button->paint();
    go_back_button->paint();

    screen::paint();
}

bool screens::join_network_game_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key == console::keyboard::character::ESCAPE)
    {
        on_navigate_up();
        return true;
    }

    const auto& focused_component = tab_indexer.get_focused_component<component>();
    if (handle_keyboard_event_for_child(key, focused_component))
    {
        return true;
    }

    if (!key.is_vertical_arrow())
    {
        return component::handle_keyboard_event(key);
    }

    const core::offset arrow_key_offset = key.get_arrow_offset();
    tab_indexer.focus_next_component(arrow_key_offset.y);

    return true;
}
