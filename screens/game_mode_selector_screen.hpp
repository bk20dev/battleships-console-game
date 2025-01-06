#pragma once
#include "../components/list.hpp"
#include "../core/component.hpp"

namespace screens
{
    class game_mode_selector_screen final : public core::component
    {
        std::shared_ptr<components::list<components::text_list_item>> game_mode_options_list;

        const std::function<void()> on_play_with_computer_selected;
        const std::function<void()> on_start_network_game_selected;
        const std::function<void()> on_join_network_game_selected;

        void initialize_game_mode_options_list();

    public:
        game_mode_selector_screen(int x, int y, const std::shared_ptr<console::console>& console,
                                  const std::function<void()>& on_play_with_computer_selected,
                                  const std::function<void()>& on_start_network_game_selected,
                                  const std::function<void()>& on_join_network_game_selected);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
