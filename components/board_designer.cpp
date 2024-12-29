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

components::board_designer::board_designer(const int x, const int y, const std::shared_ptr<console::console>& console):
    component(x, y, total_board_width, total_board_height, console)
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

void components::board_designer::paint_battleship(const battleship& battleship_to_paint) const
{
    const auto [position, size] = battleship_to_paint.rectangle;

    for (int row = 0; row < size.height; row++)
    {
        const int x = position.x * column_width;
        const int y = (position.y + row) * row_height;

        const std::string battleship_row = repeat_text("\u2593", size.width * column_width);
        const std::string styled_battleship_row = selected_ship_style.apply_to_text(battleship_row);

        console_view->write_at(x, y, styled_battleship_row);
    }
}

void components::board_designer::paint()
{
    paint_board();

    if (selected_battleship.has_value())
    {
        const battleship selected_battleship_value = selected_battleship.value();
        paint_battleship(selected_battleship_value);
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

    battleship updated_battleship = selected_battleship_value;

    if (key.character == console::keyboard::character::SPACE)
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
