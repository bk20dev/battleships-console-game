#include "board_designer_screen.hpp"

#include "../constants/style.hpp"

void screens::board_designer_screen::initialize_components()
{
    full_board_designer_picker = std::make_shared<components::full_board_designer_picker>(
        0, 0, child_console_view,
        [this](const std::vector<components::keyboard_actions::keyboard_action>& keyboard_actions)
        {
            set_keyboard_actions(keyboard_actions);
        });

    submit_placement_button = std::make_shared<components::text_button>(
        0, 12, child_console_view, "Submit", [this]
        {
            validate_and_submit_battleship_placement();
        });
    submit_placement_validation_error_label = std::make_shared<components::label>(8, 12, child_console_view, "");
    submit_placement_validation_error_label->set_style(constants::style::general::error);
}

void screens::board_designer_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(full_board_designer_picker);
    tab_indexer.connect_component(submit_placement_button);
}

void screens::board_designer_screen::display_validation_error_message(const std::string& error_message) const
{
    submit_placement_validation_error_label->set_text(error_message);

    if (submit_placement_validation_error_label->should_repaint())
    {
        submit_placement_validation_error_label->paint();
    }
}

bool screens::board_designer_screen::validate_battleship_placement() const
{
    return full_board_designer_picker->validate_all_battleships_placed() && full_board_designer_picker->
        validate_no_conflicting_battleships();
}

void screens::board_designer_screen::submit_battleship_placement(
    const std::vector<models::battleship>& placed_battleships) const
{
    // TODO:
}

void screens::board_designer_screen::validate_and_submit_battleship_placement() const
{
    const auto& placed_battleships = full_board_designer_picker->get_placed_battleships();

    if (!validate_battleship_placement())
    {
        display_validation_error_message("Place all battleships and resolve conflicts.");
        return;
    }

    display_validation_error_message("");
    submit_battleship_placement(placed_battleships);
}

screens::board_designer_screen::board_designer_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console)
    : screen(x, y, console, "Place your battleships")
{
    initialize_components();
    initialize_tab_indexer();
}

void screens::board_designer_screen::paint()
{
    full_board_designer_picker->paint();
    submit_placement_button->paint();
    screen::paint();
}

bool screens::board_designer_screen::handle_keyboard_event(const console::keyboard::key& key)
{
    if (const auto& component = tab_indexer.get_focused_component<core::component>())
    {
        if (handle_keyboard_event_for_child(key, component))
        {
            return true;
        }
    }

    if (!key.is_vertical_arrow())
    {
        return screen::handle_keyboard_event(key);
    }

    const core::offset key_arrow_offset = key.get_arrow_offset();
    tab_indexer.focus_next_component(key_arrow_offset.y);
    display_validation_error_message("");

    invalidate();
    return true;
}
