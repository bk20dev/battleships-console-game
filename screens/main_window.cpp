#include "main_window.hpp"

#include "opponent_disconnected_screen.hpp"
#include "../constants/dimension.hpp"

void screens::main_window::initialize_game_controller(const std::shared_ptr<engine::game_controller>& game_controller)
{
    this->game_controller = game_controller;
    game_controller->on_opponent_disconnected = [this]
    {
        handle_opponent_disconnected();
    };
    // game_controller->on_all_boards_ready = [this]
    // {
    //     handle_all_boards_ready();
    // };
}

void screens::main_window::handle_game_controller_ready(const std::shared_ptr<engine::game_controller>& game_controller)
{
    initialize_game_controller(game_controller);
    navigate_to(BOARD_DESIGNER_SCREEN);
}

void screens::main_window::handle_opponent_disconnected()
{
    navigate_to(OPPONENT_DISCONNECTED_SCREEN);
}

void screens::main_window::handle_exit_to_main_screen()
{
    game_controller = nullptr;
    navigate_to(GAME_SETUP_SCREEN);
}

void screens::main_window::handle_all_boards_ready()
{
    navigate_to(GAMEPLAY_SCREEN);
}

std::shared_ptr<screens::game_setup_screen> screens::main_window::create_game_setup_screen()
{
    return std::make_shared<game_setup_screen>(
        0, 0, console_view,
        [this](const std::shared_ptr<engine::game_controller>& game_controller)
        {
            handle_game_controller_ready(game_controller);
        });
}

std::shared_ptr<screens::opponent_disconnected_screen> screens::main_window::create_opponent_disconnected_screen()
{
    return std::make_shared<opponent_disconnected_screen>(
        0, 0, console_view,
        [this]
        {
            handle_exit_to_main_screen();
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
    case OPPONENT_DISCONNECTED_SCREEN:
        return create_opponent_disconnected_screen();
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
    }
    console_view->flush();

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
