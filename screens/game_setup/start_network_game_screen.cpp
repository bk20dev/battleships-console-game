#include "start_network_game_screen.hpp"

#include <iostream>

#include "../../constants/style.hpp"
#include "../../engine/serializable_peer.hpp"
#include "../../network/socket_error.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> start_network_game_screen_keyboard_actions = {
        {
            .key_to_press = components::keyboard_actions::VERTICAL_ARROWS,
            .action_description = "Move cursor",
            .key_to_press_length = components::keyboard_actions::VERTICAL_ARROWS_LENGTH,
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
        6, 0, child_console_view, components::input_type::NUMBER, 5, std::to_string(default_port_value));

    port_input_hint_label = std::make_shared<components::label>(
        0, 2, child_console_view,
        "The port number is a specific number that will allow the second\n"
        "player to connect. Suggested port numbers range from 1024 to 49151.");
    port_input_hint_label->set_style(constants::style::general::hint_style);

    start_game_button = std::make_shared<components::text_button>(
        0, 7, child_console_view, "Start game", [this]
        {
            handle_start_game_button_clicked();
        });
    go_back_button = std::make_shared<components::text_button>(
        0, 8, child_console_view, "Go back", [this]
        {
            on_navigate_up();
        });
    start_game_feedback_label = std::make_shared<components::label>(0, 5, child_console_view);
    network_details_label = std::make_shared<components::label>(0, child_console_view_height - 1, child_console_view);
    network_details_label->set_style(constants::style::general::hint_style);

    set_keyboard_actions(start_network_game_screen_keyboard_actions);
}

void screens::start_network_game_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(port_input);
    tab_indexer.connect_component(start_game_button);
    tab_indexer.connect_component(go_back_button);
}

void screens::start_network_game_screen::initialize_tcp_server() const
{
    tcp_server_connection->on_client_connected = [this]
    {
        handle_client_connected();
    };
    tcp_server_connection->on_network_error = [this](const network::socket_error& network_error)
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

void screens::start_network_game_screen::display_error_message(const std::string& error_message) const
{
    start_game_feedback_label->set_style(constants::style::general::error_style);
    display_label_message(start_game_feedback_label, error_message);
}

void screens::start_network_game_screen::display_notice_message(const std::string& notice_message) const
{
    start_game_feedback_label->set_style(constants::style::general::notice_style);
    display_label_message(start_game_feedback_label, notice_message);
}

void screens::start_network_game_screen::clear_notice_message() const
{
    display_label_message(start_game_feedback_label, "");
}

void screens::start_network_game_screen::display_network_log(const std::string& network_log) const
{
    network_details_label->set_text(network_log);
    if (network_details_label->should_repaint())
    {
        network_details_label->paint();
    }
}

void screens::start_network_game_screen::clear_network_log() const
{
    display_network_log("");
}

void screens::start_network_game_screen::handle_client_connected() const
{
    const auto server_peer = std::make_shared<engine::serializable_peer>(tcp_server_connection);
    on_peer_created(server_peer);
}

static std::string get_textual_socket_error_feedback(const network::socket_error& socket_error)
{
    using namespace network::native_socket;

    switch (socket_error.socket_error_code)
    {
    case error_port_in_use:
        return "This port is already in use. Pick a different one.";
    case error_port_unavailable:
        return "Enter a valid port number. (0–65535)";
    default:
        return "Something went wrong.";
    }
}

void screens::start_network_game_screen::handle_network_error(const network::socket_error& network_error) const
{
    stop_listening_for_players();

    const std::string textual_socket_error_feedback = get_textual_socket_error_feedback(network_error);
    display_error_message(textual_socket_error_feedback);
    display_network_log(network_error.what());
}

int screens::start_network_game_screen::get_port_value() const
{
    const std::string entered_port_text = port_input->get_text();
    if (entered_port_text.empty())
    {
        return default_port_value;
    }
    return std::stoi(entered_port_text);
}

void screens::start_network_game_screen::start_listening_for_players(const int port) const
{
    start_game_button->set_text("Cancel");
    if (start_game_button->should_repaint())
    {
        start_game_button->paint();
    }

    display_notice_message("Waiting for opponent...");
    clear_network_log();

    tcp_server_connection->start_listening(port);
}

void screens::start_network_game_screen::stop_listening_for_players() const
{
    tcp_server_connection->stop_listening();

    start_game_button->set_text("Start game");
    if (start_game_button->should_repaint())
    {
        start_game_button->paint();
    }

    clear_notice_message();
}

void screens::start_network_game_screen::handle_start_game_button_clicked() const
{
    if (tcp_server_connection->is_listening())
    {
        stop_listening_for_players();
        return;
    }

    const int selected_port = get_port_value();
    start_listening_for_players(selected_port);
}

screens::start_network_game_screen::start_network_game_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void(const std::shared_ptr<engine::i_peer>&)>& on_peer_created,
    const std::function<void()>& on_navigate_up)
    : screen(x, y, console, "Start network game"),
      on_peer_created(on_peer_created),
      on_navigate_up(on_navigate_up)
{
    initialize_components();
    initialize_tab_indexer();
    initialize_tcp_server();
}

void screens::start_network_game_screen::paint()
{
    port_input_label->paint();
    port_input->paint();
    port_input_hint_label->paint();
    start_game_button->paint();
    go_back_button->paint();
    network_details_label->paint();

    screen::paint();
}

bool screens::start_network_game_screen::handle_keyboard_event(const console::keyboard::key& key)
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

    if (tcp_server_connection->is_listening())
    {
        return false;
    }

    if (!key.is_vertical_arrow())
    {
        return component::handle_keyboard_event(key);
    }

    const core::offset arrow_key_offset = key.get_arrow_offset();
    tab_indexer.focus_next_component(arrow_key_offset.y);

    return true;
}
