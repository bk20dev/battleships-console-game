#include "battleship_selector_screen.hpp"

void screens::battleship_selector_screen::initialize_components()
{
    board_designer = std::make_shared<components::board_designer>(
        0, 0, console_view,
        placed_battleships,
        misplaced_battleships,
        [this](const models::battleship& battleship_to_place)
        {
            place_battleship(battleship_to_place);
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

void screens::battleship_selector_screen::focus_board_designer()
{
    board_designer->focus();
    battleship_selector->blur();
    paint();
}

void screens::battleship_selector_screen::focus_battleship_selector()
{
    board_designer->blur();
    battleship_selector->focus();
    paint();
}

void screens::battleship_selector_screen::erase_battleship(const models::battleship& battleship_to_find,
                                                           std::vector<models::battleship>& vector_to_search)
{
    std::erase_if(vector_to_search, [battleship_to_find](const models::battleship& current_battleship)
    {
        return current_battleship.id == battleship_to_find.id;
    });
}

void screens::battleship_selector_screen::place_battleship(const models::battleship& battleship_to_place)
{
    erase_battleship(battleship_to_place, placed_battleships);
    placed_battleships.push_back(battleship_to_place);

    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void screens::battleship_selector_screen::cancel_battleship_placement()
{
    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void screens::battleship_selector_screen::select_battleship(const models::battleship& battleship_to_select)
{
    board_designer->set_selected_battleship(battleship_to_select);
    focus_board_designer();
}

screens::battleship_selector_screen::battleship_selector_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(x, y, 0, 0, console)
{
    initialize_components();
    battleship_selector->focus();
}

void screens::battleship_selector_screen::paint()
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

bool screens::battleship_selector_screen::handle_keyboard_event(const console::keyboard::key& key)
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
