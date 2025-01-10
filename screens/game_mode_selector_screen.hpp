#pragma once
#include "../components/keyboard_actions.hpp"
#include "../components/label.hpp"
#include "../components/list.hpp"
#include "../core/component.hpp"

namespace screens
{
    class game_mode_selector_screen final : public core::component
    {
        std::shared_ptr<components::label> screen_label;
        std::shared_ptr<components::list<components::text_list_item>> game_mode_options_list;
        std::shared_ptr<components::keyboard_actions::footer_component> keyboard_actions_footer;

        const std::function<void()> on_play_with_computer_selected;
        const std::function<void()> on_start_network_game_selected;
        const std::function<void()> on_join_network_game_selected;

        void initialize_game_mode_options() const;
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
