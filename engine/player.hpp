#pragma once

#include <functional>
#include <vector>

#include "../models/battleship.hpp"
#include "../models/bullet.hpp"

namespace engine
{
    class player final
    {
        /** Tells whether the player has prepared their board (placed their battleships). */
        bool is_board_ready = false;
        /** Tells whether the player is currently making their turn. */
        bool currently_plays = false;

        /** All battleships owned by the player. */
        std::vector<models::battleship> placed_battleships{};
        /** Bullets shot by the opponent player. */
        std::vector<models::bullet> opponent_bullets{};
        /** Parts of battleships that were damaged by the opponent player. */
        std::vector<core::position> damaged_battleship_parts{};
        /** Battleships that were destroyed by the opponent player. */
        std::vector<models::battleship> destroyed_battleships{};

        std::function<void(const core::position& damaged_battleship_part)> on_add_damaged_battleship_part;
        std::function<void(const models::battleship& destroyed_battleship)> on_destroyed_battleship;

        /** Checks if a given bullet was already shot. */
        [[nodiscard]] bool is_opponent_bullet_present(const models::bullet& bullet_to_find) const;
        /** Returns a battleship at a given position. */
        [[nodiscard]] std::optional<models::battleship> get_battleship(const core::position& position) const;
        /** Checks whether a battleship was destroyed by opponent's bullets. */
        [[nodiscard]] bool is_battleship_destroyed(const models::battleship& battleship_to_check) const;

    public:
        [[nodiscard]] bool get_is_board_ready() const;

        [[nodiscard]] bool get_is_currently_playing() const;

        [[nodiscard]] const std::vector<models::battleship>& get_placed_battleships() const;

        [[nodiscard]] const std::vector<models::bullet>& get_opponent_bullets() const;

        [[nodiscard]] const std::vector<core::position>& get_damaged_battleship_parts() const;

        [[nodiscard]] const std::vector<models::battleship>& get_destroyed_battleships() const;

        [[nodiscard]] bool is_board_hit(const core::position&) const;

        void set_placed_battleships(const std::vector<models::battleship>& placed_battleships);

        bool shoot_with(const models::bullet& bullet);

        explicit player(
            const std::function<void(const core::position&)>& on_add_damaged_battleship_part = nullptr,
            const std::function<void(const models::battleship&)>& on_destroyed_battleship = nullptr);

        void add_damaged_battleship_part(const core::position& damaged_battleship_part);

        void add_destroyed_battleship(const models::battleship& destroyed_battleship);

        void set_board_ready();

        void set_currently_plays(bool currently_plays);
    };
}
