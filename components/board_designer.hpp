#pragma once

#include "../console/style.hpp"
#include "../constants/constants.hpp"
#include "../core/component.hpp"

struct battleship
{
    core::rectangle rectangle;

    battleship rotated() const
    {
        return {
            .rectangle = rectangle.rotated(),
        };
    }

    battleship moved_by(const core::offset& offset) const
    {
        return {
            .rectangle = rectangle + offset,
        };
    }

    bool operator==(const battleship& other_battleship) const
    {
        return rectangle == other_battleship.rectangle;
    }
};

namespace components
{
    class board_designer final : public core::component
    {
        static constexpr core::rectangle board_rectangle = {
            .size = {
                .width = constants::board_column_count,
                .height = constants::board_row_count,
            },
        };

        static constexpr console::style::style board_style = {
            .foreground_color = console::style::BRIGHT_BLACK,
        };

        static constexpr console::style::style selected_ship_style = {
            .foreground_color = console::style::CYAN,
        };

        static std::string repeat_text(const std::string& text, int times);

        std::optional<battleship> selected_battleship;

    public:
        board_designer(int x, int y, const std::shared_ptr<console::console>& console);

        void paint_board() const;

        void paint_battleship(const battleship& battleship_to_paint) const;

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void set_selected_battleship(const std::optional<battleship>& battleship_to_select);
    };
}
