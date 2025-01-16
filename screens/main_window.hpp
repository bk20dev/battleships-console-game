#pragma once

#include "board_designer/board_designer_screen.hpp"
#include "../core/component.hpp"
#include "game_setup/game_setup_screen.hpp"

namespace screens
{
    class main_window final : public core::component
    {
        enum destination
        {
            GAME_SETUP_SCREEN,
            BOARD_DESIGNER_SCREEN
        };

        std::shared_ptr<component> current_screen = nullptr;

        std::shared_ptr<game_setup_screen> create_game_setup_screen();
        std::shared_ptr<board_designer_screen> create_board_designer_screen();
        std::shared_ptr<component> create_screen(destination destination);

        void navigate_to(destination destination);

    public:
        main_window(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
