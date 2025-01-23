#pragma once

#include "opponent_disconnected_screen.hpp"
#include "board_designer/board_designer_screen.hpp"
#include "../core/component.hpp"
#include "gameplay/gameplay_screen.hpp"
#include "game_setup/game_setup_screen.hpp"

namespace screens
{
    class main_window final : public core::component
    {
        enum destination
        {
            GAME_SETUP_SCREEN,
            OPPONENT_DISCONNECTED_SCREEN,
            BOARD_DESIGNER_SCREEN,
            GAMEPLAY_SCREEN,
        };

        std::shared_ptr<component> current_screen = nullptr;
        std::shared_ptr<engine::game_controller> game_controller = nullptr;

        void handle_opponent_disconnected();
        void handle_exit_to_main_screen();
        void handle_all_boards_ready();

        void initialize_game_controller(const std::shared_ptr<engine::game_controller>&);
        void handle_game_controller_ready(const std::shared_ptr<engine::game_controller>&);

        std::shared_ptr<game_setup_screen> create_game_setup_screen();
        std::shared_ptr<opponent_disconnected_screen> create_opponent_disconnected_screen();
        std::shared_ptr<board_designer_screen> create_board_designer_screen();
        std::shared_ptr<gameplay_screen> create_gameplay_screen();

        std::shared_ptr<component> create_screen(destination destination);
        void navigate_to(destination destination);

    public:
        main_window(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;
        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
