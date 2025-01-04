#pragma once

#include "../core/component.hpp"
#include "../models/battleship.hpp"
#include "../models/bullet.hpp"

namespace components
{
    class player_board final : public core::component
    {
        const std::vector<models::battleship>& placed_battleships;

        const std::vector<models::bullet>& opponent_bullets;

        void paint_board() const;

        void paint_placed_battleships() const;

        bool is_battleship_hit(const models::bullet& bullet) const;

        void paint_opponent_bullet(const models::bullet& bullet) const;

        void paint_opponent_bullets() const;

    public:
        player_board(int x, int y, const std::shared_ptr<console::console>& console,
                     const std::vector<models::battleship>& placed_battleships,
                     const std::vector<models::bullet>& opponent_bullets);

        void paint() override;
    };
}
