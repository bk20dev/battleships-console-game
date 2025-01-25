#pragma once

#include <functional>

#include "../../models/battleship.hpp"

namespace engine
{
    class i_peer
    {
    public:
        std::function<void()> on_disconnect = nullptr;

        std::function<void()> on_opponent_board_prepared = nullptr;
        std::function<void(bool opponent_player)> on_turn_changed = nullptr;
        std::function<void(const models::position& position)> on_opponent_shot_received = nullptr;
        std::function<void(const models::position& damaged_battleship_part)> on_opponent_battleship_part_damaged
            = nullptr;
        std::function<void(const models::battleship& destroyed_battleship)> on_opponent_battleship_destroyed = nullptr;
        std::function<void()> on_opponent_player_lost = nullptr;

        virtual void notify_board_prepared();
        virtual void change_turn(bool current_player);
        virtual void notify_shot_fired(const models::position& position);
        virtual void notify_battleship_part_damaged(const models::position& damaged_battleship_part);
        virtual void notify_battleship_destroyed(const models::battleship& destroyed_battleship);
        virtual void notify_player_lost();

        virtual ~i_peer() = default;
    };
}
