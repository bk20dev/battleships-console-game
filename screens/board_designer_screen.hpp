#pragma once

#include "../components/battleship_selector.hpp"
#include "../components/board_designer.hpp"
#include "../constants/constants.hpp"
#include "../core/component.hpp"

namespace screens
{
    class board_designer_screen final : public core::component
    {
        std::shared_ptr<components::board_designer> board_designer;

        std::shared_ptr<components::battleship_selector> battleship_selector;

        const std::vector<models::battleship> all_battleships = std::vector(
            constants::battleships.begin(), constants::battleships.end());

        std::vector<models::battleship> placed_battleships{};

        std::vector<models::battleship> misplaced_battleships{};

        void initialize_components();

        void focus_board_designer();

        void focus_battleship_selector();

        std::optional<models::battleship> find_placed_battleship(const models::battleship& battleship_to_find) const;

        void erase_placed_battleship(const models::battleship& battleship_to_find);

        void place_battleship(const models::battleship& battleship_to_place);

        void put_back_battleship(const models::battleship& battleship_to_place);

        void cancel_battleship_placement();

        void select_battleship(const models::battleship& battleship_to_select);

    public:
        board_designer_screen(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
