#include "board_designer_screen.hpp"

#include "../utils/vector.hpp"

void screens::board_designer_screen::initialize_components()
{
    board_designer = std::make_shared<components::board_designer>(
        0, 0, console_view,
        placed_battleships,
        misplaced_battleships,
        [this](const models::battleship& battleship_to_place)
        {
            place_battleship(battleship_to_place);
        },
        [this](const models::battleship& battleship_to_put_back)
        {
            put_back_battleship(battleship_to_put_back);
        },
        [this]
        {
            cancel_battleship_placement();
        });

    battleship_selector = std::make_shared<components::battleship_selector>(
        26, 0, console_view,
        all_battleships,
        placed_battleships,
        misplaced_battleships,
        [this](const models::battleship& battleship_to_select)
        {
            select_battleship(battleship_to_select);
        });
}

void screens::board_designer_screen::focus_board_designer()
{
    board_designer->focus();
    battleship_selector->blur();
    paint();
}

void screens::board_designer_screen::focus_battleship_selector()
{
    board_designer->blur();
    battleship_selector->focus();
    paint();
}

std::optional<models::battleship> screens::board_designer_screen::find_placed_battleship(
    const models::battleship& battleship_to_find) const
{
    return utils::find_if_or_null(placed_battleships, [battleship_to_find](const models::battleship& current_battleship)
    {
        return current_battleship.id == battleship_to_find.id;
    });
}

void screens::board_designer_screen::erase_placed_battleship(const models::battleship& battleship_to_find)
{
    std::erase_if(placed_battleships, [battleship_to_find](const models::battleship& current_battleship)
    {
        return current_battleship.id == battleship_to_find.id;
    });
}

void screens::board_designer_screen::place_battleship(const models::battleship& battleship_to_place)
{
    erase_placed_battleship(battleship_to_place);
    placed_battleships.push_back(battleship_to_place);
    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void screens::board_designer_screen::put_back_battleship(const models::battleship& battleship_to_place)
{
    erase_placed_battleship(battleship_to_place);
    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void screens::board_designer_screen::cancel_battleship_placement()
{
    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void screens::board_designer_screen::select_battleship(const models::battleship& battleship_to_select)
{
    if (const auto& placed_battleship = find_placed_battleship(battleship_to_select))
    {
        board_designer->set_selected_battleship(placed_battleship);
    }
    else
    {
        board_designer->set_selected_battleship(battleship_to_select);
    }
    focus_board_designer();
}

screens::board_designer_screen::board_designer_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(x, y, 0, 0, console)
{
    initialize_components();
    battleship_selector->focus();
}

void screens::board_designer_screen::paint()
{
    board_designer->paint();
    battleship_selector->paint();

    component::paint();
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
bool handle_keyboard_event_if_focused(const std::shared_ptr<C>& component, const console::keyboard::key& key)
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

bool screens::board_designer_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (handle_keyboard_event_if_focused(board_designer, key))
    {
        return true;
    }
    if (handle_keyboard_event_if_focused(battleship_selector, key))
    {
        return true;
    }

    return component::handle_keyboard_event(key);
}
