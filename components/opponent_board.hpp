#pragma once

#include <vector>

#include "../core/component.hpp"
#include "../core/component_traits.hpp"
#include "../models/battleship.hpp"
#include "../models/bullet.hpp"

namespace components
{
    class opponent_board final : public core::component, public core::component_traits::focusable
    {
        const std::vector<models::bullet>& current_player_bullets;

        const std::vector<core::position>& revealed_battleship_parts;

        const std::vector<models::battleship>& destroyed_battleships;

        std::function<void(const core::position& position)> on_position_select;

        core::position crosshair_position = {0, 0};

        void paint_crosshair(const core::rectangle& clip_rectangle, const std::string& fill_character,
                             const console::style::style& crosshair_style) const;

        void paint_crosshair_if_focused(const core::rectangle& clip_rectangle, const std::string& fill_character,
                                        const console::style::color& foreground_color = console::style::WHITE) const;

        void paint_board() const;

        void paint_prohibited_areas() const;

        bool is_battleship_hit(const core::position& position) const;

        void paint_current_player_bullet(const models::bullet& bullet) const;

        void paint_current_player_bullets() const;

        bool is_any_object_hit(const core::position& position) const;

    public:
        opponent_board(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::vector<models::bullet>& current_player_bullets,
                       const std::vector<core::position>& revealed_battleship_parts,
                       const std::vector<models::battleship>& destroyed_battleships,
                       const std::function<void(const core::position& position)>& on_position_select);

        void paint() override;
        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void focus() override;
        void blur() override;
    };
}
