#include "board_designer_screen.hpp"

void screens::board_designer_screen::initialize_components()
{
    full_board_designer_picker = std::make_shared<components::full_board_designer_picker>(
        0, 0, child_console_view,
        [this](const std::vector<components::keyboard_actions::keyboard_action>& keyboard_actions)
        {
            set_keyboard_actions(keyboard_actions);
        });
}

void screens::board_designer_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(full_board_designer_picker);
}

screens::board_designer_screen::board_designer_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : screen(x, y, console, "Place your ships")
{
    initialize_components();
    initialize_tab_indexer();
}

void screens::board_designer_screen::paint()
{
    full_board_designer_picker->paint();
    screen::paint();
}

bool screens::board_designer_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (const auto& component = tab_indexer.get_focused_component<core::component>())
    {
        if (handle_keyboard_event_for_child(key, component))
        {
            return true;
        }
    }

    return screen::handle_keyboard_event(key);
}
