#pragma once

#include "../../components/button.hpp"
#include "../../components/full_board_designer_picker.hpp"
#include "../../components/screen.hpp"
#include "../../core/tab_indexer.hpp"
#include "../../engine/game_controller.hpp"

namespace screens
{
    class board_designer_screen final : public components::screen
    {
        bool is_board_submitted = false;
        core::tab_indexer tab_indexer{};

        std::shared_ptr<engine::game_controller> game_controller;

        std::shared_ptr<components::full_board_designer_picker> full_board_designer_picker;
        std::shared_ptr<components::label> submit_feedback_label;
        std::shared_ptr<components::text_button> submit_placement_button;

        void initialize_components();
        void initialize_tab_indexer();

        void display_notice_message(const std::string& notice_message) const;
        void display_error_message(const std::string& error_message) const;
        void clear_notice_message() const;
        bool validate_battleship_placement() const;
        void submit_battleship_placement(const std::vector<models::battleship>& placed_battleships);
        void validate_and_submit_battleship_placement();

        void handle_opponent_board_prepared() const;

    public:
        board_designer_screen(int x, int y, const std::shared_ptr<console::console>& console,
                              const std::shared_ptr<engine::game_controller>& game_controller);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
