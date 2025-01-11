#pragma once

#include "../components/keyboard_actions.hpp"
#include "../components/label.hpp"
#include "../components/list.hpp"
#include "../components/screen.hpp"
#include "../core/component.hpp"

namespace screens
{
    class game_mode_selector_screen final : public components::screen
    {
        std::shared_ptr<components::list<components::text_list_item>> game_mode_list;

        const std::function<void()> on_play_with_computer_selected;
        const std::function<void()> on_start_network_game_selected;
        const std::function<void()> on_join_network_game_selected;

        void initialize_game_mode_list();

        void initialize_components();

    public:
        game_mode_selector_screen(int x, int y, const std::shared_ptr<console::console>& console,
                                  const std::function<void()>& on_play_with_computer_selected,
                                  const std::function<void()>& on_start_network_game_selected,
                                  const std::function<void()>& on_join_network_game_selected);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
