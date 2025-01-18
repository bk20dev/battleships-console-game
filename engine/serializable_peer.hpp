#pragma once

#include <iostream>

#include "game_object_serializer.hpp"
#include "message_serializer.hpp"
#include "interfaces/i_connection.hpp"
#include "interfaces/i_peer.hpp"

namespace engine
{
    class serializable_peer final : public i_peer
    {
        const game_object_serializer game_object_serializer{};
        const message_serializer message_serializer{};

        std::shared_ptr<i_connection> connection;

        void send_message(message_serializer::message_type, const std::string& serialized_game_object = "") const;
        void handle_message(const std::string& serialized_message) const;

    public:
        explicit serializable_peer(std::shared_ptr<i_connection> connection);

        void notify_board_prepared() override;
        void change_turn(bool current_player) override;
        void notify_shot_fired(const core::position& shot_position) override;
        void notify_battleship_part_damaged(const core::position& damaged_battleship_part) override;
        void notify_battleship_destroyed(const models::battleship& destroyed_battleship) override;

        void handle_opponent_board_prepared() const;
        void handle_turn_changed(const std::string& serialized_game_object) const;
        void handle_opponent_shot_received(const std::string& serialized_game_object) const;
        void handle_opponent_battleship_part_damaged(const std::string& serialized_game_object) const;
        void handle_opponent_battleship_destroyed(const std::string& serialized_game_object) const;
    };
}
