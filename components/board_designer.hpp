#pragma once

#include "../console/style.hpp"
#include "../constants/constants.hpp"
#include "../core/component.hpp"

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

    bool operator==(const battleship& other_battleship) const
    {
        return id == other_battleship.id && rectangle == other_battleship.rectangle;
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

        static constexpr console::style::style default_battleship_style = {
            .foreground_color = console::style::WHITE,
        };

        static constexpr console::style::style selected_battleship_style = {
            .foreground_color = console::style::CYAN,
        };

        static std::string repeat_text(const std::string& text, int times);

        std::function<void(const battleship& battleship)> on_submit_placement;

        std::function<void()> on_cancel_placement;

        std::optional<battleship> selected_battleship;

        bool is_battleship_selected(const battleship& battleship_to_check) const;

    public:
        board_designer(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::function<void(const battleship& battleship)>& on_submit_placement,
                       const std::function<void()>& on_cancel_placement);

        std::vector<battleship> battleships{};


        void paint_board() const;

        void paint_battleship(const battleship& battleship_to_paint, const console::style::style& style) const;

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void set_selected_battleship(const std::optional<battleship>& battleship_to_select);
    };
}
