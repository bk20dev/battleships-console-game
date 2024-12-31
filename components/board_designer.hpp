#pragma once

#include "../console/style.hpp"
#include "../core/component.hpp"
#include "../core/component_traits.hpp"
#include "../models/battleship.hpp"

namespace components
{
    class board_designer final : public core::component, public core::component_traits::focusable
    {
        const std::vector<models::battleship>& placed_battleships;

        const std::vector<models::battleship>& misplaced_battleships;

        std::function<void(const models::battleship& battleship_placement)> on_submit_placement;

        std::function<void()> on_cancel_placement;

        std::optional<models::battleship> selected_battleship;

        bool is_battleship_selected(const models::battleship& battleship_to_check) const;

        bool is_battleship_misplaced(const models::battleship& battleship_to_check) const;

        console::style::style get_battleship_style(const models::battleship& battleship_to_style) const;

        void paint_battleship(const models::battleship& battleship_to_paint, const console::style::style& style) const;

        void paint_board() const;

    public:
        board_designer(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::vector<models::battleship>& placed_battleships,
                       const std::vector<models::battleship>& misplaced_battleships,
                       const std::function<void(const models::battleship& battleship_placement)>& on_submit_placement,
                       const std::function<void()>& on_cancel_placement);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void set_selected_battleship(const std::optional<models::battleship>& battleship_to_select);
    };
}
