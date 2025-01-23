#pragma once

#include "../components/button.hpp"
#include "../components/screen.hpp"

namespace screens
{
    class opponent_disconnected_screen final : public components::screen
    {
        const std::function<void()> on_exit_to_main_menu;

        std::shared_ptr<components::text_button> exit_to_main_menu_button = nullptr;

        void handle_exit_to_main_menu_clicked() const;

        void initialize_components();

    public:
        opponent_disconnected_screen(int x, int y, const std::shared_ptr<console::console>& console,
                                     const std::function<void()>& on_exit_to_main_menu);

        bool handle_keyboard_event(const console::keyboard::key& key) override;
        void paint() override;
    };
}
