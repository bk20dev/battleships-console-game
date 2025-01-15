#pragma once

#include <functional>

#include "../core/dimension.hpp"
#include "../models/battleship.hpp"

namespace engine
{
    class i_peer
    {
    public:
        std::function<void()> on_opponent_board_prepared;
        std::function<void(bool opponent_player)> on_turn_changed;
        std::function<void(const core::position& position)> on_opponent_shot_received;
        std::function<void(const core::position& damaged_battleship_part)> on_opponent_battleship_part_damaged;
        std::function<void(const models::battleship& destroyed_battleship)> on_opponent_battleship_destroyed;

        virtual void notify_board_prepared();
        virtual void change_turn(bool current_player);
        virtual void notify_shot_fired(const core::position& position);
        virtual void notify_battleship_part_damaged(const core::position& damaged_battleship_part);
        virtual void notify_battleship_destroyed(const models::battleship& destroyed_battleship);

        virtual ~i_peer() = default;
    };
}
