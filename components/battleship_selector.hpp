#pragma once

#include "board_designer.hpp"
#include "list.hpp"
#include "../core/component.hpp"

namespace components
{
    class battleship_selector_item final : public list_item
    {
        static constexpr console::style::style default_battleship_item_style = {
        };

        static constexpr console::style::style selected_battleship_item_style = {
            .foreground_color = console::style::color::CYAN
        };

        battleship normalized_battleship;

    public:
        battleship_selector_item(int x, int y, const std::shared_ptr<console::console>& console,
                                 const battleship& battleship, const std::function<void()>& on_select);

        void paint() override;

        void focus() override;

        void blur() override;
    };
}
