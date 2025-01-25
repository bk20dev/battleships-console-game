#pragma once

#include "../../components/opponent_board.hpp"
#include "../../components/player_board.hpp"
#include "../../components/remaining_battleship_viewer.hpp"
#include "../../components/screen.hpp"
#include "../../core/component.hpp"
#include "../../engine/game_controller.hpp"

namespace screens
{
    class gameplay_screen final : public components::screen
    {
        const std::shared_ptr<engine::game_controller> game_controller;

        std::shared_ptr<components::label> current_player_board_label;
        std::shared_ptr<components::label> opponent_board_label;
        std::shared_ptr<components::player_board> current_player_board;
        std::shared_ptr<components::opponent_board> opponent_board;
        std::shared_ptr<components::remaining_battleship_viewer> opponent_remaining_battleships_viewer;

        void initialize_components();

        void handle_turn_changed(bool current_player) const;
        void handle_current_player_board_updated() const;
        void handle_opponent_player_board_updated() const;

        void shoot_opponent_board(const core::position& crosshair_position) const;

    public:
        gameplay_screen(int x, int y, const std::shared_ptr<console::console>& console,
                        const std::shared_ptr<engine::game_controller>& game_controller);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
