#pragma once

#include "player.hpp"
#include "interfaces/i_peer.hpp"
#include "../constants/gameplay.hpp"

namespace engine
{
    class ai_peer final : public i_peer
    {
        const std::vector<models::battleship> available_battleships{
            constants::gameplay::battleships.begin(),
            constants::gameplay::battleships.end()
        };

        player ai_player;
        player opponent_player;

        static std::vector<models::position> generate_shot_positions(const models::size& board_size);
        std::vector<models::position> positions_left_to_shoot{};

        void shoot_opponent_board();

        void handle_opponent_player_board_prepared();
        void handle_ai_player_turn();
        void handle_ai_player_battleship_part_damaged(const models::position&) const;
        void handle_ai_player_battleship_destroyed(const models::battleship&) const;
        void handle_ai_player_game_lost() const;

        void initialize_players();

    public:
        ai_peer();

        void notify_board_prepared() override;
        void change_turn(bool current_player) override;
        void notify_shot_fired(const models::position& crosshair_position) override;
        void notify_battleship_part_damaged(const models::position& damaged_battleship_part) override;
        void notify_battleship_destroyed(const models::battleship& destroyed_battleship) override;
    };
}
