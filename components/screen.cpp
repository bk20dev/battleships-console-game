#include "screen.hpp"

#include "../constants/dimension.hpp"

void components::screen::initialize_screen_title_label(const std::string& screen_title)
{
    screen_title_label = std::make_shared<label>(0, 2, console_view, screen_title);
}

void components::screen::initialize_keyboard_actions_footer()
{
    const int y = size.height - 1;

    keyboard_actions_footer = std::make_shared<keyboard_actions::footer_component>(0, y, size.width, console_view);
}

void components::screen::initialize_child_console_view()
{
    static constexpr core::offset child_console_view_offset = {.x = 0, .y = 5};
    child_console_view = console_view->create_view(child_console_view_offset);
}

void components::screen::paint_components() const
{
    screen_title_label->paint();
    keyboard_actions_footer->paint();
}

void components::screen::set_keyboard_actions(
    const std::vector<keyboard_actions::keyboard_action>& keyboard_actions) const
{
    keyboard_actions_footer->set_actions(keyboard_actions);

    if (keyboard_actions_footer->should_repaint())
    {
        keyboard_actions_footer->paint();
    }
}

components::screen::screen(
    const int x, const int y, const std::shared_ptr<console::console>& console, const std::string& screen_title)
    : component(x, y, constants::dimension::screen_width, constants::dimension::screen_height, console)
{
    initialize_screen_title_label(screen_title);
    initialize_keyboard_actions_footer();

    initialize_child_console_view();
}

void components::screen::paint()
{
    paint_components();
    component::paint();
}
