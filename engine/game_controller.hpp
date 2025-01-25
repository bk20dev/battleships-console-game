#pragma once

#include "player.hpp"
#include "../components/battleship.hpp"
#include "interfaces/i_peer.hpp"

namespace engine
{
    class game_controller final
    {
        player current_player;
        player opponent_player;

        bool game_over = false;

        std::shared_ptr<i_peer> opponent_peer_connection = nullptr;

        void change_turn(bool current_player_turn);

        void handle_all_battleship_placements_submitted() const;
        void handle_any_battleship_placement_submitted() const;

        void handle_opponent_player_disconnected() const;
        void handle_opponent_player_battleship_placement_submitted();
        void handle_turn_changed(bool current_player_turn);
        void handle_current_player_board_updated() const;
        void handle_opponent_player_shot_received(const core::position&);
        void handle_current_player_battleship_part_damaged(const core::position&) const;
        void handle_current_player_battleship_destroyed(const models::battleship&) const;
        void handle_opponent_player_battleship_part_damaged(const core::position&);
        void handle_opponent_player_battleship_destroyed(const models::battleship&);
        void handle_game_over(bool current_player_won);
        void handle_current_player_game_lost();
        void handle_opponent_player_game_lost();

        void initialize_players();
        void setup_opponent_peer_connection(const std::shared_ptr<i_peer>&);

    public:
        explicit game_controller(const std::shared_ptr<i_peer>& opponent_peer_connection);

        std::function<void()> on_opponent_disconnected = nullptr;
        std::function<void()> on_opponent_battleship_placement_submitted = nullptr;
        std::function<void()> on_all_battleship_placements_submitted = nullptr;
        std::function<void(bool current_player_turn)> on_turn_changed = nullptr;
        std::function<void()> on_current_player_board_updated = nullptr;
        std::function<void()> on_opponent_board_updated = nullptr;
        std::function<void(bool current_player_won)> on_game_over = nullptr;

        void submit_current_player_battleship_placement(const std::vector<models::battleship>&);
        void shoot_opponent_player(const core::position&);

        [[nodiscard]] bool is_opponent_battleship_placement_submitted() const;
        [[nodiscard]] bool is_current_player_turn() const;
        [[nodiscard]] const player& get_current_player() const;
        [[nodiscard]] const player& get_opponent_player() const;
        [[nodiscard]] bool is_game_over() const;
    };
}
