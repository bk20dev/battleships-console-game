#pragma once

#include "../console/style.hpp"
#include "../core/component.hpp"
#include "../core/component_traits.hpp"

struct battleship
{
    int id = -1;
    core::rectangle rectangle;

    battleship rotated() const
    {
        return {
            .id = id,
            .rectangle = rectangle.rotated(),
        };
    }

    battleship moved_by(const core::offset& offset) const
    {
        return {
            .id = id,
            .rectangle = rectangle + offset,
        };
    }

    battleship normalized() const
    {
        return {
            .id = id,
            .rectangle = rectangle.normalized(),
        };
    }

    bool operator==(const battleship& other_battleship) const
    {
        return id == other_battleship.id && rectangle == other_battleship.rectangle;
    }
};

namespace components
{
    class board_designer final : public core::component, public core::component_traits::focusable
    {
        const std::vector<battleship>& placed_battleships;

        const std::vector<battleship>& misplaced_battleships;

        std::function<void(const battleship& battleship)> on_submit_placement;

        std::function<void()> on_cancel_placement;

        std::optional<battleship> selected_battleship;

        bool is_battleship_selected(const battleship& battleship_to_check) const;

        bool is_battleship_misplaced(const battleship& battleship_to_check) const;

        console::style::style get_battleship_style(const battleship& battleship_to_style) const;

        void paint_battleship(const battleship& battleship_to_paint, const console::style::style& style) const;

        void paint_board() const;

    public:
        board_designer(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::vector<battleship>& placed_battleships,
                       const std::vector<battleship>& misplaced_battleships,
                       const std::function<void(const battleship& battleship)>& on_submit_placement,
                       const std::function<void()>& on_cancel_placement);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void set_selected_battleship(const std::optional<battleship>& battleship_to_select);
    };
}
