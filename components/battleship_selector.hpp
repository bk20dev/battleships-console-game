#pragma once

#include <iostream>

#include "board_designer.hpp"
#include "list.hpp"
#include "../core/component.hpp"

namespace components
{
    class battleship_selector final : public core::component, public core::component_traits::focusable
    {
        const std::vector<models::battleship>& all_battleships;

        const std::vector<models::battleship>& placed_battleships;

        const std::vector<models::battleship>& misplaced_battleships;

        std::function<void(const models::battleship& battleship_to_select)> on_battleship_select;

        int selected_battleship_index = 0;

        bool is_battleship_placed(const models::battleship& battleship_to_check) const;

        bool is_battleship_misplaced(const models::battleship& battleship_to_check) const;

        void paint_battleships() const;

        bool update_selected_battleship_index(int selected_index_delta);

        std::optional<models::battleship> get_selected_battleship() const;

    public:
        battleship_selector(int x, int y, const std::shared_ptr<console::console>& console,
                            const std::vector<models::battleship>& all_battleships,
                            const std::vector<models::battleship>& placed_battleships,
                            const std::vector<models::battleship>& misplaced_battleships,
                            const std::function<void(const models::battleship& battleship_to_select)>&
                            on_battleship_select);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
