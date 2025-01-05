#pragma once

#include "../components/opponent_board.hpp"
#include "../components/player_board.hpp"
#include "../core/component.hpp"

namespace screens
{
    class gameplay_screen final : public core::component
    {
        std::shared_ptr<components::opponent_board> opponent_board;
        std::shared_ptr<components::player_board> player_board;

        /** Bullets shot by the current player. */
        std::vector<models::bullet> current_player_bullets{};
        /** Battleships owned by the current player. */
        std::vector<models::battleship> current_player_battleships{};

        /** Bullets shot by the opponent player. */
        std::vector<models::bullet> opponent_bullets{};
        /** Parts of opponent's battleships shot down by the current player. */
        std::vector<core::position> opponent_revealed_battleship_parts{};
        /** Destroyed battleships owned by the opponent player. */
        std::vector<models::battleship> opponent_destroyed_battleships{};

        void initialize_components();

        void shot_opponent_board(const core::position& position);

    public:
        gameplay_screen(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
