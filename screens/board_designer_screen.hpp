#pragma once

#include "../components/button.hpp"
#include "../components/full_board_designer_picker.hpp"
#include "../components/screen.hpp"
#include "../core/tab_indexer.hpp"

namespace screens
{
    class board_designer_screen final : public components::screen
    {
        core::tab_indexer tab_indexer{};

        std::shared_ptr<components::full_board_designer_picker> full_board_designer_picker;
        std::shared_ptr<components::text_button> submit_placement_button;
        std::shared_ptr<components::label> submit_placement_validation_error_label;

        void initialize_components();
        void initialize_tab_indexer();

        void display_validation_error_message(const std::string& error_message) const;
        bool validate_battleship_placement() const;
        void submit_battleship_placement(const std::vector<models::battleship>& placed_battleships) const;
        void validate_and_submit_battleship_placement() const;

    public:
        board_designer_screen(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
