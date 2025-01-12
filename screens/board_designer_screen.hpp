#pragma once

#include "../components/full_board_designer_picker.hpp"
#include "../components/screen.hpp"
#include "../core/tab_indexer.hpp"

namespace screens
{
    class board_designer_screen final : public components::screen
    {
        core::tab_indexer tab_indexer{};

        std::shared_ptr<components::full_board_designer_picker> full_board_designer_picker;

        void initialize_components();
        void initialize_tab_indexer();

    public:
        board_designer_screen(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
