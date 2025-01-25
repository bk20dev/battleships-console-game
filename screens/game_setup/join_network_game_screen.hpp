#pragma once

#include "../../components/input.hpp"
#include "../../components/screen.hpp"
#include "../../core/tab_indexer.hpp"
#include "../../engine/interfaces/i_peer.hpp"
#include "../../network/tcp_client.hpp"

namespace components
{
    class text_button;
}

namespace screens
{
    class join_network_game_screen final : public components::screen
    {
        static constexpr int default_port_value = 3000;

        core::tab_indexer tab_indexer{};

        std::shared_ptr<components::label> ip_input_label;
        std::shared_ptr<components::input> ip_input;
        std::shared_ptr<components::label> port_input_label;
        std::shared_ptr<components::input> port_input;
        std::shared_ptr<components::text_button> join_game_button;
        std::shared_ptr<components::text_button> go_back_button;
        std::shared_ptr<components::label> join_game_feedback_label;
        std::shared_ptr<components::label> network_log_label;

        std::shared_ptr<network::tcp_client_connection> tcp_client_connection
            = std::make_shared<network::tcp_client_connection>();

        const std::function<void(const std::shared_ptr<engine::i_peer>&)> on_peer_created;
        const std::function<void()> on_navigate_up;

        void initialize_components();
        void initialize_tab_indexer();
        void initialize_tcp_client() const;

        void display_error_message(const std::string& error_message) const;
        void display_notice_message(const std::string& notice_message) const;
        void clear_notice_message() const;
        void display_network_log(const std::string&) const;
        void clear_network_log() const;

        void handle_server_connected() const;
        void handle_network_error(const network::socket_error&) const;

        [[nodiscard]] int get_port_value() const;
        [[nodiscard]] std::string get_ip_address_value() const;

        void join_game(const std::string& ip_address, int port) const;
        void leave_game() const;
        void handle_join_game_button_clicked() const;

    public:
        ~join_network_game_screen() override;

        join_network_game_screen(int x, int y, const std::shared_ptr<console::console>& console,
                                 const std::function<void(const std::shared_ptr<engine::i_peer>&)>& on_peer_created,
                                 const std::function<void()>& on_navigate_up);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
