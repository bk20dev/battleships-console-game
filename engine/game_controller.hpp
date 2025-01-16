#pragma once

#include "player.hpp"
#include "interfaces/i_peer.hpp"

namespace engine
{
    class game_controller final
    {
        std::shared_ptr<i_peer> peer_connection;

        player current_player;
        player opponent_player;

        void initialize_players();

        [[nodiscard]] bool is_opponent_bullet_present(const models::bullet& bullet_to_find) const;

        void peer_notify_board_prepared() const;
        void peer_change_turn(bool current_player) const;
        void peer_fire_shot(const core::position& position) const;
        void peer_notify_battleship_part_damaged(const core::position& damaged_battleship_part) const;
        void peer_notify_battleship_destroyed(const models::battleship& destroyed_battleship) const;

        static void handle_opponent_board_prepared();
        void handle_turn_changed(bool opponent_player);
        void handle_opponent_shot_received(const core::position& position);
        void handle_opponent_battleship_part_damaged(const core::position& damaged_battleship_part);
        void handle_opponent_battleship_destroyed(const models::battleship& destroyed_battleship);

    public:
        explicit game_controller(const std::shared_ptr<i_peer>& peer_connection);

        void set_placed_battleships(const std::vector<models::battleship>& current_player_placed_battleships);

        std::function<void(bool current_player)> on_turn_changed = nullptr;

        void change_turn(bool selected_player);
        bool shoot_opponent_board(const core::position& position);

        [[nodiscard]] const player& get_current_player() const;
        [[nodiscard]] const player& get_opponent_player() const;
    };
}
