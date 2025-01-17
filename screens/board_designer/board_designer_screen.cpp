#include "board_designer_screen.hpp"

#include "../../constants/style.hpp"

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
    submit_feedback_label = std::make_shared<components::label>(8, 12, child_console_view, "");
}

void screens::board_designer_screen::initialize_tab_indexer()
{
    tab_indexer.connect_component(full_board_designer_picker);
    tab_indexer.connect_component(submit_placement_button);
}

void display_label_message(std::shared_ptr<components::label> target_label, const std::string& message)
{
    target_label->set_text(message);

    if (target_label->should_repaint())
    {
        target_label->paint();
    }
}

void screens::board_designer_screen::display_validation_error_message(const std::string& error_message) const
{
    submit_feedback_label->set_style(constants::style::general::error_style);
    display_label_message(submit_feedback_label, error_message);
}

void screens::board_designer_screen::display_notice_message(const std::string& notice_message) const
{
    submit_feedback_label->set_style(constants::style::general::notice_style);
    display_label_message(submit_feedback_label, notice_message);
}

bool screens::board_designer_screen::validate_battleship_placement() const
{
    return full_board_designer_picker->validate_all_battleships_placed() && full_board_designer_picker->
        validate_no_conflicting_battleships();
}

void screens::board_designer_screen::submit_battleship_placement(
    const std::vector<models::battleship>& placed_battleships)
{
    game_controller->set_placed_battleships(placed_battleships);
    game_controller->mark_board_prepared();
    is_board_submitted = true;
}

void screens::board_designer_screen::validate_and_submit_battleship_placement()
{
    if (is_board_submitted)
    {
        return;
    }

    const auto& placed_battleships = full_board_designer_picker->get_placed_battleships();

    if (!validate_battleship_placement())
    {
        display_validation_error_message("Place all battleships and resolve conflicts.");
        return;
    }

    display_notice_message("Waiting for opponent...");
    submit_battleship_placement(placed_battleships);
}

screens::board_designer_screen::board_designer_screen(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::shared_ptr<engine::game_controller>& game_controller)
    : screen(x, y, console, "Place your battleships"), game_controller(game_controller)
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
    if (is_board_submitted)
    {
        return false;
    }

    const auto& component = tab_indexer.get_focused_component<core::component>();
    if (handle_keyboard_event_for_child(key, component))
    {
        return true;
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
