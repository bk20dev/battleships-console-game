#include "board_designer.hpp"

#include "../constants/constants.hpp"
#include "../utils/text.hpp"

namespace
{
    constexpr int row_height = 1;
    constexpr int column_width = 2;

    constexpr int total_board_width = constants::board_column_count * column_width;
    constexpr int total_board_height = constants::board_row_count * row_height;

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

    constexpr console::style::style default_battleship_style = {
        .foreground_color = console::style::WHITE,
    };

    constexpr console::style::style selected_battleship_style = {
        .foreground_color = console::style::CYAN,
    };

    constexpr console::style::style misplaced_battleship_style = {
        .foreground_color = console::style::RED,
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

console::style::style components::board_designer::get_battleship_style(
    const models::battleship& battleship_to_style) const
{
    if (is_battleship_misplaced(battleship_to_style))
    {
        return misplaced_battleship_style;
    }
    return default_battleship_style;
}

void components::board_designer::paint_battleship(
    const models::battleship& battleship_to_paint, const console::style::style& style) const
{
    const auto [position, size] = battleship_to_paint.rectangle;

    for (int row = 0; row < size.height; row++)
    {
        const int x = position.x * column_width;
        const int y = (position.y + row) * row_height;

        const std::string battleship_row = utils::repeat_string("\u2593", size.width * column_width);
        const std::string styled_battleship_row = style.apply_to_text(battleship_row);

        console_view->write_at(x, y, styled_battleship_row);
    }
}

void components::board_designer::paint_board() const
{
    for (int row = 0; row < size.height; row++)
    {
        const std::string board_row = utils::repeat_string("\u2591", constants::board_column_count * column_width);
        const std::string styled_board_row = board_style.apply_to_text(board_row);

        console_view->write_at(0, row, styled_board_row);
    }
}

void components::board_designer::paint()
{
    paint_board();

    for (const auto& placed_battleship : placed_battleships)
    {
        if (is_battleship_selected(placed_battleship))
        {
            continue;
        }

        console::style::style battleship_style = get_battleship_style(placed_battleship);
        paint_battleship(placed_battleship, battleship_style);
    }

    if (selected_battleship.has_value())
    {
        const models::battleship selected_battleship_value = selected_battleship.value();
        paint_battleship(selected_battleship_value, selected_battleship_style);
    }

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
