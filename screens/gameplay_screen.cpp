#include "gameplay_screen.hpp"

#include "../components/list.hpp"

void screens::gameplay_screen::initialize_components()
{
    opponent_board = std::make_shared<components::opponent_board>(
        0, 0, console_view,
        current_player_bullets,
        opponent_revealed_battleship_parts,
        opponent_destroyed_battleships,
        [this](const core::position& position)
        {
            shot_opponent_board(position);
        });

    player_board = std::make_shared<components::player_board>(
        26, 0, console_view,
        current_player_battleships,
        opponent_bullets);
}

void screens::gameplay_screen::shot_opponent_board(const core::position& position)
{
    // TODO:
}

screens::gameplay_screen::gameplay_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(x, y, 0, 0, console)
{
    initialize_components();
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
static bool handle_keyboard_event_if_focused(const std::shared_ptr<C>& component, const console::keyboard::key& key)
{
    if (!component->get_is_focused())
    {
        return false;
    }
    if (component->handle_keyboard_event(key))
    {
        if (component->should_repaint())
        {
            component->paint();
        }
    }
    return true;
}

void screens::gameplay_screen::paint()
{
    opponent_board->paint();
    player_board->paint();

    component::paint();
}

bool screens::gameplay_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (handle_keyboard_event_if_focused(opponent_board, key))
    {
        return true;
    }
    return component::handle_keyboard_event(key);
}
