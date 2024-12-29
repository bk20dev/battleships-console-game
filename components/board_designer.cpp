#include "board_designer.hpp"

#include "../constants/constants.hpp"

namespace
{
    constexpr int row_height = 1;
    constexpr int column_width = 2;

    constexpr int total_board_width = constants::board_column_count * column_width;
    constexpr int total_board_height = constants::board_row_count * row_height;
}

std::string components::board_designer::repeat_text(const std::string& text, const int times)
{
    std::string result_text = "";
    result_text.reserve(text.size() * times);

    for (int i = 0; i < times; i++)
    {
        result_text += text;
    }

    return result_text;
}

bool components::board_designer::is_battleship_selected(const battleship& battleship_to_check) const
{
    if (selected_battleship.has_value())
    {
        const battleship selected_battleship_value = selected_battleship.value();
        if (selected_battleship_value.id == battleship_to_check.id)
        {
            return true;
        }
    }
    return false;
}

components::board_designer::board_designer(const int x, const int y, const std::shared_ptr<console::console>& console,
                                           const std::function<void(const battleship& battleship)>& on_submit_placement,
                                           const std::function<void()>& on_cancel_placement)
    : component(x, y, total_board_width, total_board_height, console),
      on_submit_placement(on_submit_placement), on_cancel_placement(on_cancel_placement)
{
}

void components::board_designer::paint_board() const
{
    for (int row = 0; row < size.height; row++)
    {
        const std::string board_row = repeat_text("\u2591", constants::board_column_count * column_width);
        const std::string styled_board_row = board_style.apply_to_text(board_row);

        console_view->write_at(0, row, styled_board_row);
    }
}

void components::board_designer::paint_battleship(
    const battleship& battleship_to_paint, const console::style::style& style) const
{
    const auto [position, size] = battleship_to_paint.rectangle;

    for (int row = 0; row < size.height; row++)
    {
        const int x = position.x * column_width;
        const int y = (position.y + row) * row_height;

        const std::string battleship_row = repeat_text("\u2593", size.width * column_width);
        const std::string styled_battleship_row = style.apply_to_text(battleship_row);

        console_view->write_at(x, y, styled_battleship_row);
    }
}

void components::board_designer::paint()
{
    paint_board();

    for (const auto& battleship : battleships)
    {
        if (is_battleship_selected(battleship))
        {
            continue;
        }

        console::style::style battleship_style = default_battleship_style;
        paint_battleship(battleship, battleship_style);
    }

    if (selected_battleship.has_value())
    {
        const battleship selected_battleship_value = selected_battleship.value();
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

    const battleship selected_battleship_value = selected_battleship.value();

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

    battleship updated_battleship = selected_battleship_value;

    if (key == console::keyboard::character::SPACE)
    {
        const battleship rotated_battleship = updated_battleship.rotated();
        updated_battleship = rotated_battleship;
    }

    if (key.is_arrow())
    {
        const core::offset battleship_position_offset = key.get_arrow_offset();
        const battleship moved_battleship = updated_battleship.moved_by(battleship_position_offset);
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

void components::board_designer::set_selected_battleship(const std::optional<battleship>& battleship_to_select)
{
    selected_battleship = battleship_to_select;
    invalidate();
}
