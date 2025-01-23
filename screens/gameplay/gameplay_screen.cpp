#include "gameplay_screen.hpp"

#include "../../components/list.hpp"

void screens::gameplay_screen::initialize_components()
{
    const auto& current_player = game_controller->get_current_player();
    const auto& opponent_player = game_controller->get_opponent_player();

    opponent_board = std::make_shared<components::opponent_board>(
        0, 0, console_view,
        opponent_player.get_opponent_bullets(),
        opponent_player.get_damaged_battleship_parts(),
        opponent_player.get_destroyed_battleships(),
        [this](const core::position& crosshair_position)
        {
            shoot_opponent_board(crosshair_position);
        });

    player_board = std::make_shared<components::player_board>(
        26, 0, console_view,
        current_player.get_placed_battleships(),
        current_player.get_opponent_bullets());
}

void screens::gameplay_screen::handle_turn_changed(const bool current_player) const
{
    if (current_player)
    {
        opponent_board->focus();
    }
    else
    {
        opponent_board->blur();
    }

    if (opponent_board->should_repaint())
    {
        opponent_board->paint();
    }
}

void screens::gameplay_screen::shoot_opponent_board(const core::position& crosshair_position) const
{
    game_controller->shoot_opponent_player(crosshair_position);
}

screens::gameplay_screen::gameplay_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::shared_ptr<engine::game_controller>& game_controller)
    : component(x, y, 0, 0, console), game_controller(game_controller)
{
    initialize_components();

    game_controller->on_turn_changed = [this](const bool current_player_turn)
    {
        handle_turn_changed(current_player_turn);
    };
    handle_turn_changed(game_controller->is_current_player_turn());
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
