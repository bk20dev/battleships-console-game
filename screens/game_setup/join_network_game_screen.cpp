#include "join_network_game_screen.hpp"

#include "../../components/button.hpp"
#include "../../constants/style.hpp"
#include "../../engine/serializable_peer.hpp"
#include "../../network/socket_error.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> default_keyboard_actions = {
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

    const std::vector<components::keyboard_actions::keyboard_action> connecting_to_server_keyboard_actions = {
        {
            .key_to_press = "Enter",
            .action_description = "Cancel",
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
    join_game_feedback_label = std::make_shared<components::label>(0, 3, child_console_view);
    join_game_button = std::make_shared<components::text_button>(0, 5, child_console_view, "Join game", [this]
    {
        handle_join_game_button_clicked();
    });
    go_back_button = std::make_shared<components::text_button>(0, 6, child_console_view, "Go back", [this]
    {
        on_navigate_up();
    });
    network_log_label = std::make_shared<components::label>(0, child_console_view_height - 1, child_console_view);
    network_log_label->set_style(constants::style::general::hint_style);

    set_keyboard_actions(default_keyboard_actions);
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
        handle_server_connected();
    };
    tcp_client_connection->on_network_error = [this](const network::socket_error& network_error)
    {
        handle_network_error(network_error);
    };
}

static void display_label_message(const std::shared_ptr<components::label>& target_label, const std::string& message)
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

void screens::join_network_game_screen::clear_notice_message() const
{
    display_label_message(join_game_feedback_label, "");
}

void screens::join_network_game_screen::display_network_log(const std::string& network_log) const
{
    network_log_label->set_text(network_log);
    if (network_log_label->should_repaint())
    {
        network_log_label->paint();
    }
}

void screens::join_network_game_screen::clear_network_log() const
{
    display_network_log("");
}

void screens::join_network_game_screen::handle_server_connected() const
{
    const auto client_peer = std::make_shared<engine::serializable_peer>(tcp_client_connection);
    on_peer_created(client_peer);
}

static std::string get_textual_socket_error_feedback(const network::socket_error& socket_error)
{
    using namespace network::native_socket;

    switch (socket_error.socket_error_code)
    {
    case error_port_unavailable:
        return "Enter a valid port number. (0–65535)";
    case error_timed_out:
        return "Server not found. Make sure the opponent's IP address matches the entered one.";
    case error_connection_refused:
        return "Connection refused. Make sure the opponent's port matches the entered one.";
    default:
        return "Something went wrong.";
    }
}

void screens::join_network_game_screen::handle_network_error(const network::socket_error& network_error) const
{
    leave_game();

    const std::string textual_socket_error_feedback = get_textual_socket_error_feedback(network_error);
    display_error_message(textual_socket_error_feedback);
    display_network_log(network_error.what());
}

int screens::join_network_game_screen::get_port_value() const
{
    const std::string entered_port_text = port_input->get_text();
    if (entered_port_text.empty())
    {
        return default_port_value;
    }
    return std::stoi(entered_port_text);
}

std::string screens::join_network_game_screen::get_ip_address_value() const
{
    return ip_input->get_text();
}

void screens::join_network_game_screen::join_game(const std::string& ip_address, const int port) const
{
    join_game_button->set_text("Cancel");
    if (join_game_button->should_repaint())
    {
        join_game_button->paint();
    }

    display_notice_message("Connecting...");
    clear_network_log();

    set_keyboard_actions(connecting_to_server_keyboard_actions);

    tcp_client_connection->connect_to(ip_address, port);
}

void screens::join_network_game_screen::leave_game() const
{
    display_notice_message("Disconnecting...");
    tcp_client_connection->disconnect();

    join_game_button->set_text("Start game");
    if (join_game_button->should_repaint())
    {
        join_game_button->paint();
    }

    clear_notice_message();

    set_keyboard_actions(default_keyboard_actions);
}

void screens::join_network_game_screen::handle_join_game_button_clicked() const
{
    if (tcp_client_connection->is_listening())
    {
        leave_game();
        return;
    }

    const std::string selected_ip_address = get_ip_address_value();
    const int selected_port = get_port_value();
    join_game(selected_ip_address, selected_port);
}

screens::join_network_game_screen::~join_network_game_screen()
{
    tcp_client_connection->on_server_connected = nullptr;
    tcp_client_connection->on_network_error = nullptr;
}

screens::join_network_game_screen::join_network_game_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void(const std::shared_ptr<engine::i_peer>&)>& on_peer_created,
    const std::function<void()>& on_navigate_up)
    : screen(x, y, console, "Join network game"),
      on_peer_created(on_peer_created), on_navigate_up(on_navigate_up)
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
