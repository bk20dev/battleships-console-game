#include "game_mode_selector_screen.hpp"

#include <iostream>

namespace
{
    constexpr int total_screen_width = 0;
    constexpr int total_screen_height = 0;
}

void screens::game_mode_selector_screen::initialize_game_mode_options_list()
{
    game_mode_options_list = std::make_shared<components::list<components::text_list_item>>(0, 0, console_view);

    const auto play_with_computer_option = std::make_shared<components::text_list_item>(
        0, 0, nullptr, "Play with computer", [this]
        {
            if (on_play_with_computer_selected)
            {
                on_play_with_computer_selected();
            }
        });
    const auto start_network_game = std::make_shared<components::text_list_item>(
        0, 1, nullptr, "Start a network game", [this]
        {
            if (on_start_network_game_selected)
            {
                on_start_network_game_selected();
            }
        });
    const auto join_network_game = std::make_shared<components::text_list_item>(
        0, 2, nullptr, "Join a network game", [this]
        {
            if (on_join_network_game_selected)
            {
                on_join_network_game_selected();
            }
        });

    game_mode_options_list->add_component(play_with_computer_option);
    game_mode_options_list->add_component(start_network_game);
    game_mode_options_list->add_component(join_network_game);
}

screens::game_mode_selector_screen::game_mode_selector_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_play_with_computer_selected,
    const std::function<void()>& on_start_network_game_selected,
    const std::function<void()>& on_join_network_game_selected)
    : component(x, y, total_screen_width, total_screen_height, console),
      on_play_with_computer_selected(on_play_with_computer_selected),
      on_start_network_game_selected(on_start_network_game_selected),
      on_join_network_game_selected(on_join_network_game_selected)
{
    initialize_game_mode_options_list();
}

void screens::game_mode_selector_screen::paint()
{
    game_mode_options_list->paint();

    component::paint();
}

bool screens::game_mode_selector_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    return game_mode_options_list->handle_keyboard_event(key);
}
