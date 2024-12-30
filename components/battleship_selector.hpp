#pragma once

#include <iostream>

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

    class battleship_selector final : public core::component
    {
        std::vector<battleship> battleships;
        std::shared_ptr<list<battleship_selector_item>> battleship_selector_list;

        std::function<void(const battleship& battleship)> on_battleship_select;

        void add_battleships_to_selector_list(const std::vector<battleship>& battleships_to_add);

        void select_battleship(const battleship& battleship_to_select) const;

    public:
        battleship_selector(int x, int y, const std::shared_ptr<console::console>& console,
                            const std::vector<battleship>& battleships,
                            const std::shared_ptr<list<battleship_selector_item>>& battleship_selector_list,
                            const std::function<void(const battleship& battleship)>& on_battleship_select);

        battleship_selector(int x, int y, const std::shared_ptr<console::console>& console,
                            const std::vector<battleship>& battleships,
                            const std::function<void(const battleship& battleship)>& on_battleship_select);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
