#include "full_board_designer_picker.hpp"

#include "../constants/dimension.hpp"
#include "../utils/vector.tpp"

void components::full_board_designer_picker::initialize_board_designer()
{
    board_designer = std::make_shared<components::board_designer>(
        0, 0, console_view,
        placed_battleships,
        conflicting_battleships,
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
}

void components::full_board_designer_picker::initialize_battleship_selector()
{
    battleship_selector = std::make_shared<components::battleship_selector>(
        26, 0, console_view,
        all_battleships,
        placed_battleships,
        conflicting_battleships,
        [this](const models::battleship& battleship_to_select)
        {
            select_battleship(battleship_to_select);
        });
}

void components::full_board_designer_picker::initialize_components()
{
    initialize_board_designer();
    initialize_battleship_selector();
}

void components::full_board_designer_picker::focus_component(
    const std::shared_ptr<focusable>& component_to_focus)
{
    if (focused_component)
    {
        focused_component->blur();
    }

    if (component_to_focus)
    {
        component_to_focus->focus();
    }

    focused_component = component_to_focus;

    paint();
}

void components::full_board_designer_picker::focus_board_designer()
{
    focus_component(board_designer);
}

void components::full_board_designer_picker::focus_battleship_selector()
{
    focus_component(battleship_selector);
}

void components::full_board_designer_picker::place_battleship(const models::battleship& battleship_to_place)
{
    erase_placed_battleship(battleship_to_place);
    placed_battleships.push_back(battleship_to_place);
    board_designer->set_selected_battleship(std::nullopt);

    conflicting_battleships = find_conflicting_battleships();

    focus_battleship_selector();
}

void components::full_board_designer_picker::put_back_battleship(const models::battleship& battleship_to_place)
{
    erase_placed_battleship(battleship_to_place);
    board_designer->set_selected_battleship(std::nullopt);

    conflicting_battleships = find_conflicting_battleships();

    focus_battleship_selector();
}

void components::full_board_designer_picker::cancel_battleship_placement()
{
    board_designer->set_selected_battleship(std::nullopt);
    focus_battleship_selector();
}

void components::full_board_designer_picker::select_battleship(const models::battleship& battleship_to_select)
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

void components::full_board_designer_picker::erase_placed_battleship(const models::battleship& battleship_to_erase)
{
    std::erase_if(placed_battleships, [battleship_to_erase](const models::battleship& current_battleship)
    {
        return current_battleship.id == battleship_to_erase.id;
    });
}

std::optional<models::battleship> components::full_board_designer_picker::find_placed_battleship(
    const models::battleship& battleship_to_find) const
{
    return utils::find_if_or_null(placed_battleships, [battleship_to_find](const models::battleship& current_battleship)
    {
        return current_battleship.id == battleship_to_find.id;
    });
}

bool components::full_board_designer_picker::check_battleship_conflicts(
    const models::battleship& battleship_to_check) const
{
    const core::rectangle prohibited_zone = battleship_to_check.rectangle.expanded_by(1);

    return std::ranges::any_of(
        placed_battleships,
        [battleship_to_check, prohibited_zone](const models::battleship& placed_battleship)
        {
            if (battleship_to_check.id == placed_battleship.id) return false;
            return placed_battleship.rectangle.intersects(prohibited_zone);
        });
}

std::vector<models::battleship> components::full_board_designer_picker::find_conflicting_battleships() const
{
    std::vector<models::battleship> conflicting_battleships{};

    for (const auto& placed_battleship : placed_battleships)
    {
        if (check_battleship_conflicts(placed_battleship))
        {
            conflicting_battleships.push_back(placed_battleship);
        }
    }

    return conflicting_battleships;
}

components::full_board_designer_picker::full_board_designer_picker(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : component(x, y, 0, constants::dimension::board_row_count, console)
{
    initialize_components();
}

void components::full_board_designer_picker::paint()
{
    board_designer->paint();
    battleship_selector->paint();

    component::paint();
}

bool components::full_board_designer_picker::handle_keyboard_event(const console::keyboard::key& key)
{
    if (const auto& component = std::dynamic_pointer_cast<core::component>(focused_component))
    {
        if (component->handle_keyboard_event(key))
        {
            if (component->should_repaint())
            {
                component->paint();
            }
            return true;
        }
    }

    return component::handle_keyboard_event(key);
}

void components::full_board_designer_picker::focus()
{
    focus_component(battleship_selector);
    focusable::focus();
}

void components::full_board_designer_picker::blur()
{
    focus_component(nullptr);
    focusable::blur();
}
