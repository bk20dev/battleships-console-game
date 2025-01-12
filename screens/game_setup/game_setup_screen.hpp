#pragma once
#include "game_mode_selector_screen.hpp"
#include "join_network_game_screen.hpp"
#include "start_network_game_screen.hpp"
#include "../../core/component.hpp"

namespace screens
{
    class game_setup_screen final : public core::component
    {
        enum destination
        {
            GAME_MODE_SELECTOR_SCREEN,
            START_NETWORK_GAME_SCREEN,
            JOIN_NETWORK_GAME_SCREEN,
        };

        const std::function<void()> on_connection_established;

        std::shared_ptr<component> current_screen = nullptr;

        std::shared_ptr<game_mode_selector_screen> create_game_mode_selector_screen();
        std::shared_ptr<start_network_game_screen> create_start_network_game_screen();
        std::shared_ptr<join_network_game_screen> create_join_network_game_screen();

        std::shared_ptr<component> create_screen(destination destination);

        void navigate_to(destination destination);

    public:
        game_setup_screen(int x, int y, const std::shared_ptr<console::console>& console,
                          const std::function<void()>& on_connection_established);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
