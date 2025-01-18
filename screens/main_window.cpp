#include "main_window.hpp"

#include "../constants/dimension.hpp"

std::shared_ptr<screens::game_setup_screen> screens::main_window::create_game_setup_screen()
{
    return std::make_shared<game_setup_screen>(
        0, 0, console_view,
        [this](const std::shared_ptr<engine::game_controller>& game_controller)
        {
            handle_game_controller_ready(game_controller);
        });
}

std::shared_ptr<screens::board_designer_screen> screens::main_window::create_board_designer_screen()
{
    return std::make_shared<board_designer_screen>(0, 0, console_view, game_controller);
}

std::shared_ptr<screens::gameplay_screen> screens::main_window::create_gameplay_screen()
{
    if (!game_controller)
    {
        throw std::runtime_error("Game controller is not initialized.");
    }
    return std::make_shared<gameplay_screen>(0, 0, console_view, game_controller);
}

std::shared_ptr<core::component> screens::main_window::create_screen(const destination destination)
{
    switch (destination)
    {
    case GAME_SETUP_SCREEN:
        return create_game_setup_screen();
    case BOARD_DESIGNER_SCREEN:
        return create_board_designer_screen();
    case GAMEPLAY_SCREEN:
        return create_gameplay_screen();
    // ReSharper disable once CppDFAUnreachableCode
    default:
        return nullptr;
    }
}

void screens::main_window::navigate_to(const destination destination)
{
    const auto& destination_screen = create_screen(destination);
    current_screen = destination_screen;

    paint();
}

void screens::main_window::handle_game_controller_ready(
    const std::shared_ptr<engine::game_controller>& prepared_game_controller)
{
    game_controller = prepared_game_controller;
    game_controller->on_all_boards_ready = [this]
    {
        handle_all_boards_ready();
    };

    navigate_to(BOARD_DESIGNER_SCREEN);
}

void screens::main_window::handle_all_boards_ready()
{
    navigate_to(GAMEPLAY_SCREEN);
}

screens::main_window::main_window(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(x, y, constants::dimension::screen_width, constants::dimension::screen_height, console)
{
    navigate_to(GAME_SETUP_SCREEN);
}

void screens::main_window::paint()
{
    console_view->clear();

    if (current_screen)
    {
        current_screen->paint();
        console_view->flush();
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
