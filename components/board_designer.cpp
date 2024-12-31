#include "board_designer.hpp"

#include "battleship.hpp"
#include "../constants/constants.hpp"
#include "../utils/text.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};

    constexpr int total_board_width = constants::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::board_row_count * pixel_size.height;

    constexpr core::rectangle board_rectangle = {
        .size = {
            .width = constants::board_column_count,
            .height = constants::board_row_count,
        },
    };
}

namespace
{
    constexpr console::style::style board_style = {
        .foreground_color = console::style::BRIGHT_BLACK,
    };
}

bool components::board_designer::is_battleship_selected(const models::battleship& battleship_to_check) const
{
    if (selected_battleship.has_value())
    {
        const models::battleship selected_battleship_value = selected_battleship.value();
        if (selected_battleship_value.id == battleship_to_check.id)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
bool vector_has_if(const std::vector<T>& vector, const std::function<bool(const T&)>& predicate)
{
    const auto found_element = std::find_if(vector.begin(), vector.end(), predicate);
    return found_element != vector.end();
}

bool components::board_designer::is_battleship_misplaced(const models::battleship& battleship_to_check) const
{
    const std::function predicate = [battleship_to_check](const models::battleship& battleship)
    {
        return battleship_to_check.id == battleship.id;
    };
    return vector_has_if(misplaced_battleships, predicate);
}

components::board_designer::board_designer(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<models::battleship>& placed_battleships,
    const std::vector<models::battleship>& misplaced_battleships,
    const std::function<void(const models::battleship& battleship)>& on_submit_placement,
    const std::function<void()>& on_cancel_placement
) : component(x, y, total_board_width, total_board_height, console),
    placed_battleships(placed_battleships), misplaced_battleships(misplaced_battleships),
    on_submit_placement(on_submit_placement), on_cancel_placement(on_cancel_placement)
{
}

void components::board_designer::paint_board() const
{
    static constexpr core::rectangle board_fill_rectangle = {
        .size = {
            .height = total_board_height,
            .width = total_board_width,
        },
    };

    const std::string board_style_sequence = board_style.to_control_sequence();

    console_view->fill_rectangle(board_fill_rectangle, board_fill_character, board_style_sequence);
}

void components::board_designer::paint_battleships() const
{
    for (const auto& placed_battleship : placed_battleships)
    {
        if (is_battleship_selected(placed_battleship))
        {
            continue;
        }

        const bool is_misplaced = is_battleship_misplaced(placed_battleship);
        battleship::paint(console_view, placed_battleship, false, is_misplaced, pixel_size);
    }

    if (selected_battleship.has_value())
    {
        const models::battleship selected_battleship_value = selected_battleship.value();
        battleship::paint(console_view, selected_battleship_value, true, false, pixel_size);
    }
}

void components::board_designer::paint()
{
    paint_board();
    paint_battleships();

    component::paint();
}

bool components::board_designer::handle_keyboard_event(const console::keyboard::key& key)
{
    if (!selected_battleship.has_value())
    {
        return false;
    }

    const models::battleship selected_battleship_value = selected_battleship.value();

    if (key == console::keyboard::character::ENTER)
    {
        if (on_submit_placement)
        {
            on_submit_placement(selected_battleship_value);
        }
        return true;
    }

    if (key == console::keyboard::character::ESCAPE)
    {
        if (on_cancel_placement)
        {
            on_cancel_placement();
        }
        return true;
    }

    models::battleship updated_battleship = selected_battleship_value;

    if (key == console::keyboard::character::SPACE)
    {
        const models::battleship rotated_battleship = updated_battleship.rotated();
        updated_battleship = rotated_battleship;
    }

    if (key.is_arrow())
    {
        const core::offset battleship_position_offset = key.get_arrow_offset();
        const models::battleship moved_battleship = updated_battleship.moved_by(battleship_position_offset);
        updated_battleship = moved_battleship;
    }

    // Ensure the selected battleship stays on the board
    updated_battleship.rectangle = updated_battleship.rectangle.fitted_into(board_rectangle);

    if (updated_battleship == selected_battleship_value)
    {
        return component::handle_keyboard_event(key);
    }

    selected_battleship = updated_battleship;

    invalidate();
    return true;
}

void components::board_designer::set_selected_battleship(const std::optional<models::battleship>& battleship_to_select)
{
    selected_battleship = battleship_to_select;
    invalidate();
}
