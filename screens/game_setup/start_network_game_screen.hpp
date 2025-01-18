#pragma once

#include "../../components/button.hpp"
#include "../../components/input.hpp"
#include "../../components/label.hpp"
#include "../../components/screen.hpp"
#include "../../core/component.hpp"
#include "../../core/tab_indexer.hpp"

#include "../../engine/interfaces/i_peer.hpp"
#include "../../network/tcp_server.hpp"

namespace screens
{
    class start_network_game_screen final : public components::screen
    {
        core::tab_indexer tab_indexer{};

        std::shared_ptr<components::label> port_input_label;
        std::shared_ptr<components::input> port_input;
        std::shared_ptr<components::label> port_input_hint_label;
        std::shared_ptr<components::text_button> start_game_button;
        std::shared_ptr<components::text_button> go_back_button;
        std::shared_ptr<components::label> network_log_label;

        std::shared_ptr<network::tcp_server> tcp_server = std::make_shared<network::tcp_server>();

        const std::function<void(const std::shared_ptr<engine::i_peer>&)> on_peer_created;
        const std::function<void()> on_navigate_up;

        void initialize_components();
        void initialize_tab_indexer();
        void initialize_tcp_server() const;

        void display_network_log(const std::string&) const;
        void handle_start_game_button_clicked();

    public:
        start_network_game_screen(int x, int y, const std::shared_ptr<console::console>& console,
                                  const std::function<void(const std::shared_ptr<engine::i_peer>&)>& on_peer_created,
                                  const std::function<void()>& on_navigate_up);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
