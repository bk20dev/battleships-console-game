#include "game_setup_screen.hpp"

#include "../../constants/dimension.hpp"

std::shared_ptr<screens::game_mode_selector_screen> screens::game_setup_screen::create_game_mode_selector_screen()
{
    return std::make_shared<game_mode_selector_screen>(
        0, 0, console_view,
        []
        {
            // TODO:
        },
        [this]
        {
            navigate_to(START_NETWORK_GAME_SCREEN);
        },
        [this]
        {
            navigate_to(JOIN_NETWORK_GAME_SCREEN);
        });
}

std::shared_ptr<screens::start_network_game_screen> screens::game_setup_screen::create_start_network_game_screen()
{
    return std::make_shared<start_network_game_screen>(
        0, 0, console_view,
        [this]
        {
            navigate_to(GAME_MODE_SELECTOR_SCREEN);
        });
}

std::shared_ptr<screens::join_network_game_screen> screens::game_setup_screen::create_join_network_game_screen()
{
    return std::make_shared<join_network_game_screen>(
        0, 0, console_view,
        [this]
        {
            navigate_to(GAME_MODE_SELECTOR_SCREEN);
        });
}

std::shared_ptr<core::component> screens::game_setup_screen::create_screen(const destination destination)
{
    switch (destination)
    {
    case GAME_MODE_SELECTOR_SCREEN:
        return create_game_mode_selector_screen();
    case START_NETWORK_GAME_SCREEN:
        return create_start_network_game_screen();
    case JOIN_NETWORK_GAME_SCREEN:
        return create_join_network_game_screen();
    // ReSharper disable once CppDFAUnreachableCode
    default:
        return nullptr;
    }
}

void screens::game_setup_screen::navigate_to(destination destination)
{
    const auto& destination_screen = create_screen(destination);
    current_screen = destination_screen;

    paint();
}

screens::game_setup_screen::game_setup_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_connection_established)
    : component(x, y, constants::dimension::screen_width, constants::dimension::screen_height, console),
      on_connection_established(on_connection_established)
{
    navigate_to(JOIN_NETWORK_GAME_SCREEN);
}

void screens::game_setup_screen::paint()
{
    console_view->clear();

    if (current_screen)
    {
        current_screen->paint();
    }

    component::paint();
}

bool screens::game_setup_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (handle_keyboard_event_for_child(key, current_screen))
    {
        return true;
    }

    return component::handle_keyboard_event(key);
}
