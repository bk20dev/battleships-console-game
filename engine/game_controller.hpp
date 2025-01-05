#pragma once

#include <vector>

#include "../models/battleship.hpp"
#include "../models/bullet.hpp"

namespace engine
{
    class game_controller final
    {
    public:
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

        void shot_opponent_board(const core::position& position);
    };
}
