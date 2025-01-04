#pragma once

#include <vector>

#include "../core/component.hpp"
#include "../models/battleship.hpp"
#include "../models/bullet.hpp"

namespace components
{
    class opponent_board final : public core::component
    {
        const std::vector<models::bullet>& current_player_bullets;

        const std::vector<core::position>& revealed_battleship_parts;

        const std::vector<models::battleship>& destroyed_battleships;

        void paint_board() const;

        void paint_prohibited_areas() const;

        bool is_battleship_hit(const models::bullet& bullet) const;

        void paint_current_player_bullet(const models::bullet& bullet) const;

        void paint_current_player_bullets() const;

    public:
        opponent_board(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::vector<models::bullet>& current_player_bullets,
                       const std::vector<core::position>& revealed_battleship_parts,
                       const std::vector<models::battleship>& destroyed_battleships);

        void paint() override;
    };
}
