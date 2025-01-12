#pragma once

#include "../../components/opponent_board.hpp"
#include "../../components/player_board.hpp"
#include "../../core/component.hpp"
#include "../../engine/game_controller.hpp"

namespace screens
{
    class gameplay_screen final : public core::component
    {
        const std::shared_ptr<engine::game_controller> game_controller;

        std::shared_ptr<components::opponent_board> opponent_board;

        std::shared_ptr<components::player_board> player_board;

        void initialize_components();

        void shoot_opponent_board(const core::position& position) const;

    public:
        gameplay_screen(int x, int y, const std::shared_ptr<console::console>& console,
                        const std::shared_ptr<engine::game_controller>& game_controller);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
