#include "join_network_game_screen.hpp"

#include "../../components/button.hpp"
#include "../../constants/style.hpp"
#include "../../engine/serializable_peer.hpp"
#include "../../network/socket_error.hpp"

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
    ip_input = std::make_shared<components::input>(11, 0, child_console_view, components::TEXT, 15, "0.0.0.0");
    port_input_label = std::make_shared<components::label>(0, 1, child_console_view, "Port");
    port_input = std::make_shared<components::input>(5, 1, child_console_view, components::NUMBER, 5,
                                                     std::to_string(default_port_value));
    join_game_button = std::make_shared<components::text_button>(0, 4, child_console_view, "Join game", [this]
    {
        handle_join_game_button_clicked();
    });
    go_back_button = std::make_shared<components::text_button>(0, 5, child_console_view, "Go back", [this]
    {
        on_navigate_up();
    });
    join_game_feedback_label = std::make_shared<components::label>(10, 4, child_console_view);
    network_log_label = std::make_shared<components::label>(0, child_console_view_height - 1, child_console_view);
    network_log_label->set_style(constants::style::general::hint_style);
    set_keyboard_actions(join_network_game_screen_keyboard_actions);
}

void screens::join_network_game_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(ip_input);
    tab_indexer.connect_component(port_input);
    tab_indexer.connect_component(join_game_button);
    tab_indexer.connect_component(go_back_button);
}

void screens::join_network_game_screen::initialize_tcp_client() const
{
    tcp_client_connection->on_server_connected = [this]
    {
        display_network_log("Connected to opponent.");
        const auto client_peer = std::make_shared<engine::serializable_peer>(tcp_client_connection);
        on_peer_created(client_peer);
    };
}

static void display_label_message(std::shared_ptr<components::label> target_label, const std::string& message)
{
    target_label->set_text(message);

    if (target_label->should_repaint())
    {
        target_label->paint();
    }
}


void screens::join_network_game_screen::display_error_message(const std::string& error_message) const
{
    join_game_feedback_label->set_style(constants::style::general::error_style);
    display_label_message(join_game_feedback_label, error_message);
}

void screens::join_network_game_screen::display_notice_message(const std::string& notice_message) const
{
    join_game_feedback_label->set_style(constants::style::general::notice_style);
    display_label_message(join_game_feedback_label, notice_message);
}

void screens::join_network_game_screen::display_network_log(const std::string& network_log) const
{
    network_log_label->set_text(network_log);
    if (network_log_label->should_repaint())
    {
        network_log_label->paint();
    }
}

void screens::join_network_game_screen::handle_join_game_button_clicked() const
{
    int port = default_port_value;

    const std::string entered_port_text = port_input->get_text();
    if (entered_port_text.length() > 0)
    {
        port = std::stoi(entered_port_text);
    }

    const std::string entered_ip_text = ip_input->get_text();

    try
    {
        display_notice_message("Connecting...");
        tcp_client_connection->connect_to(entered_ip_text, port);
    }
    catch (const network::socket_error& error)
    {
        switch (error.socket_error_code)
        {
        case 0:
            display_error_message("Invalid port value. Try a lower one.");
            break;
        case 60:
            display_error_message("Server not found. Try a different IP.");
            break;
        case 61:
            display_error_message("Server is not running or is listening on a different port.");
            break;
        default:
            display_error_message("Something went wrong.");
            break;
        }
        display_network_log(error.what());
    }
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
    initialize_tcp_client();
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
