#pragma once

#include "battleship_selector.hpp"
#include "board_designer.hpp"
#include "../constants/gameplay.hpp"
#include "../core/component.hpp"

namespace components
{
    class full_board_designer_picker final : public core::component, public core::component_traits::focusable
    {
        std::shared_ptr<board_designer> board_designer;
        std::shared_ptr<battleship_selector> battleship_selector;

        std::shared_ptr<focusable> focused_component = nullptr;

        const std::vector<models::battleship> all_battleships = std::vector(
            constants::gameplay::battleships.begin(), constants::gameplay::battleships.end());

        std::vector<models::battleship> placed_battleships{};
        std::vector<models::battleship> conflicting_battleships{};

        void initialize_board_designer();
        void initialize_battleship_selector();
        void initialize_components();

        void focus_component(const std::shared_ptr<focusable>& component_to_focus);
        void focus_board_designer();
        void focus_battleship_selector();

        void place_battleship(const models::battleship& battleship_to_place);
        void put_back_battleship(const models::battleship& battleship_to_place);
        void cancel_battleship_placement();

        void select_battleship(const models::battleship& battleship_to_select);

        void erase_placed_battleship(const models::battleship& battleship_to_erase);
        std::optional<models::battleship> find_placed_battleship(const models::battleship& battleship_to_find) const;

        bool check_battleship_conflicts(const models::battleship& battleship_to_check) const;
        std::vector<models::battleship> find_conflicting_battleships() const;

    public:
        full_board_designer_picker(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void focus() override;

        void blur() override;
    };
}
