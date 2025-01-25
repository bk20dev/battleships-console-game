#include "gameplay_screen.hpp"

#include "../../components/list.hpp"
#include "../../constants/gameplay.hpp"

namespace
{
    const std::vector<components::keyboard_actions::keyboard_action> opponent_board_keyboard_actions = {
        {
            .key_to_press = components::keyboard_actions::ARROWS,
            .action_description = "Move cursor",
            .key_to_press_length = components::keyboard_actions::ARROWS_LENGTH
        },
        {
            .key_to_press = "Enter",
            .action_description = "Shoot",
        },
    };

    const std::vector<components::keyboard_actions::keyboard_action> game_end_keyboard_actions = {
        {
            .key_to_press = "Enter",
            .action_description = "Exit",
        },
    };
}

void screens::gameplay_screen::initialize_components()
{
    current_player_board_label = std::make_shared<components::label>(0, 0, child_console_view, "Opponent's board");
    opponent_board_label = std::make_shared<components::label>(38, 0, child_console_view, "Your board");

    const auto& current_player = game_controller->get_current_player();
    const auto& opponent_player = game_controller->get_opponent_player();

    current_player_board = std::make_shared<components::player_board>(
        38, 2, child_console_view,
        current_player.get_placed_battleships(),
        current_player.get_opponent_bullets());

    opponent_board = std::make_shared<components::opponent_board>(
        0, 2, child_console_view,
        opponent_player.get_opponent_bullets(),
        opponent_player.get_damaged_battleship_parts(),
        opponent_player.get_destroyed_battleships(),
        [this](const models::position& crosshair_position)
        {
            shoot_opponent_board(crosshair_position);
        });

    opponent_remaining_battleships_viewer = std::make_shared<components::remaining_battleship_viewer>(
        24, 2, child_console_view,
        std::vector(constants::gameplay::battleships.begin(), constants::gameplay::battleships.end()),
        opponent_player.get_destroyed_battleships());

    turn_label = std::make_shared<components::turn_label>(0, 14, size.width, child_console_view);
}

void screens::gameplay_screen::handle_current_player_turn() const
{
    opponent_board->focus();
    turn_label->set_current_player_turn();
    set_keyboard_actions(opponent_board_keyboard_actions);
}

void screens::gameplay_screen::handle_opponent_player_turn() const
{
    opponent_board->blur();
    turn_label->set_opponent_player_turn();
    set_keyboard_actions({});
}

void screens::gameplay_screen::handle_turn_changed(const bool current_player) const
{
    if (game_controller->is_game_over())
    {
        return;
    }

    if (current_player)
    {
        handle_current_player_turn();
    }
    else
    {
        handle_opponent_player_turn();
    }

    opponent_board->paint();
    turn_label->paint();
    console_view->flush();
}

void screens::gameplay_screen::handle_current_player_board_updated() const
{
    current_player_board->paint();
    console_view->flush();
}

void screens::gameplay_screen::handle_opponent_player_board_updated() const
{
    opponent_board->paint();
    opponent_remaining_battleships_viewer->paint();
    console_view->flush();
}

void screens::gameplay_screen::handle_game_over(const bool current_player_won)
{
    opponent_board->blur();
    set_keyboard_actions(game_end_keyboard_actions);

    if (current_player_won)
    {
        turn_label->set_current_player_won();
    }
    else
    {
        turn_label->set_current_player_lost();
    }

    paint();
    console_view->flush();
}

void screens::gameplay_screen::shoot_opponent_board(const models::position& crosshair_position) const
{
    game_controller->shoot_opponent_player(crosshair_position);
}

screens::gameplay_screen::~gameplay_screen()
{
    game_controller->on_turn_changed = nullptr;
    game_controller->on_current_player_board_updated = nullptr;
    game_controller->on_opponent_board_updated = nullptr;
    game_controller->on_game_over = nullptr;
}

screens::gameplay_screen::gameplay_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::shared_ptr<engine::game_controller>& game_controller,
    const std::function<void()>& on_exit)
    : screen(x, y, console, "Destroy opponent's battleships"),
      game_controller(game_controller),
      on_exit(on_exit)
{
    initialize_components();

    game_controller->on_turn_changed = [this](const bool current_player_turn)
    {
        handle_turn_changed(current_player_turn);
    };
    game_controller->on_current_player_board_updated = [this]
    {
        handle_current_player_board_updated();
    };
    game_controller->on_opponent_board_updated = [this]
    {
        handle_opponent_player_board_updated();
    };
    game_controller->on_game_over = [this](const bool current_player_won)
    {
        handle_game_over(current_player_won);
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
    current_player_board_label->paint();
    opponent_board_label->paint();

    current_player_board->paint();
    opponent_board->paint();
    opponent_remaining_battleships_viewer->paint();
    turn_label->paint();

    screen::paint();
}

bool screens::gameplay_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (game_controller->is_game_over())
    {
        if (key == console::keyboard::character::ENTER)
        {
            if (on_exit)
            {
                on_exit();
            }
        }
        return false;
    }
    if (handle_keyboard_event_if_focused(opponent_board, key))
    {
        return true;
    }
    return screen::handle_keyboard_event(key);
}
