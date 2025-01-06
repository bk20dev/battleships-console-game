#pragma once
#include "player.hpp"

namespace engine
{
    class game_controller final
    {
        player current_player;

        player opponent_player;

        void initialize_players();

        bool is_opponent_bullet_present(const models::bullet& bullet_to_find) const;

        void remote_set_currently_plays(bool currently_plays);

        void remote_shoot_opponent_board(const core::position& position);

        void remote_add_damaged_battleship_part(const core::position& damaged_battleship_part);

        void remote_add_destroyed_battleship(const models::battleship& destroyed_battleship);

        void handle_shoot_current_player_board(const core::position& position);

        void handle_add_damaged_opponent_battleship_part(const core::position& damaged_battleship_part);

        void handle_destroyed_opponent_battleship(const models::battleship& destroyed_battleship);

    public:
        std::function<void(bool current_player)> on_set_player = nullptr;

        explicit game_controller(const std::vector<models::battleship>& current_player_placed_battleships);

        void set_player(bool selected_player);

        bool shoot_opponent_board(const core::position& position);

        const player& get_current_player() const;

        const player& get_opponent_player() const;
    };
}
