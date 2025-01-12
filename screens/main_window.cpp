#include "main_window.hpp"

#include "../constants/dimension.hpp"

std::shared_ptr<screens::game_setup_screen> screens::main_window::create_game_setup_screen() const
{
    return std::make_shared<game_setup_screen>(0, 0, console_view, []
    {
    });
}

std::shared_ptr<core::component> screens::main_window::create_screen(const destination destination) const
{
    switch (destination)
    {
    case GAME_SETUP_SCREEN:
        return create_game_setup_screen();
    // ReSharper disable once CppDFAUnreachableCode
    default:
        return nullptr;
    }
}

void screens::main_window::navigate_to(destination destination)
{
    const auto& destination_screen = create_screen(destination);
    current_screen = destination_screen;

    paint();
}

screens::main_window::main_window(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(0, 0, constants::dimension::screen_width, constants::dimension::screen_height, console)
{
    navigate_to(GAME_SETUP_SCREEN);
}

void screens::main_window::paint()
{
    if (current_screen)
    {
        current_screen->paint();
    }

    component::paint();
}

bool screens::main_window::handle_keyboard_event(const console::keyboard::key& key)
{
    if (handle_keyboard_event_for_child(key, current_screen))
    {
        return true;
    }

    return component::handle_keyboard_event(key);
}
