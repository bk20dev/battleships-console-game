#pragma once

#include "player.hpp"
#include "interfaces/i_peer.hpp"

namespace engine
{
    class game_controller final
    {
        //     [[nodiscard]] bool is_opponent_bullet_present(const models::bullet& bullet_to_find) const;
        //
        //     void peer_notify_board_prepared() const;
        //     void peer_change_turn(bool current_player) const;
        //     void peer_fire_shot(const core::position& position) const;
        //     void peer_notify_battleship_part_damaged(const core::position& damaged_battleship_part) const;
        //     void peer_notify_battleship_destroyed(const models::battleship& destroyed_battleship) const;
        //
        //     void handle_turn_changed(bool opponent_player);
        //     void handle_opponent_shot_received(const core::position& position);
        //     void handle_opponent_battleship_part_damaged(const core::position& damaged_battleship_part);
        //     void handle_opponent_battleship_destroyed(const models::battleship& destroyed_battleship);

        player current_player;
        player opponent_player;

        std::shared_ptr<i_peer> opponent_peer_connection = nullptr;

        void change_turn(bool current_player_turn);

        void handle_all_battleship_placements_submitted() const;
        void handle_any_battleship_placement_submitted() const;

        void handle_opponent_disconnected() const;
        void handle_opponent_battleship_placement_submitted();
        void handle_turn_changed(bool current_player_turn);

        void initialize_players();
        void setup_opponent_peer_connection(const std::shared_ptr<i_peer>&);

    public:
        explicit game_controller(const std::shared_ptr<i_peer>& opponent_peer_connection);

        std::function<void()> on_opponent_disconnected = nullptr;
        std::function<void()> on_opponent_battleship_placement_submitted = nullptr;
        std::function<void()> on_all_battleship_placements_submitted = nullptr;
        std::function<void(bool current_player_turn)> on_turn_changed = nullptr;

        void submit_current_player_battleship_placement(const std::vector<models::battleship>&);

        [[nodiscard]] bool is_opponent_battleship_placement_submitted() const;
        [[nodiscard]] bool is_current_player_turn() const;

        //     std::function<void(bool current_player)> on_turn_changed = nullptr;
        //
        //     void change_turn(bool selected_player, bool force = false);
        //     bool shoot_opponent_board(const core::position& position) const;
        //
        //     [[nodiscard]] const player& get_current_player() const;
        //     [[nodiscard]] const player& get_opponent_player() const;
    };
}
