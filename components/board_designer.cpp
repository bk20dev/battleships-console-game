#include "board_designer.hpp"

#include "battleship.hpp"
#include "../constants/dimension.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr models::size pixel_size = {.width = 2, .height = 1};

    constexpr int total_board_width = constants::dimension::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::dimension::board_row_count * pixel_size.height;

    constexpr models::rectangle board_rectangle = {
        .size = {
            .width = constants::dimension::board_column_count,
            .height = constants::dimension::board_row_count,
        },
    };
}

bool components::board_designer::is_battleship_selected(const models::battleship& battleship_to_check) const
{
    if (selected_battleship)
    {
        return selected_battleship->id == battleship_to_check.id;
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
    const std::function<void(const models::battleship& battleship)>& on_reset_placement,
    const std::function<void()>& on_cancel_placement
) : component(x, y, total_board_width, total_board_height, console),
    placed_battleships(placed_battleships), misplaced_battleships(misplaced_battleships),
    on_submit_placement(on_submit_placement), on_reset_placement(on_reset_placement),
    on_cancel_placement(on_cancel_placement)
{
}

void components::board_designer::paint_board() const
{
    static constexpr models::rectangle board_fill_rectangle = {
        .size = {
            .height = total_board_height,
            .width = total_board_width,
        },
    };

    const std::string board_style_sequence = constants::style::board::default_style.to_control_sequence();
    console_view->fill_rectangle(board_fill_rectangle, constants::style::tertiary_fill_character, board_style_sequence);
}

console::style::style get_battleship_style(const bool is_misplaced)
{
    using namespace constants::style::battleship;
    if (is_misplaced) return misplaced_style;
    return default_style;
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
        const console::style::style battleship_style = get_battleship_style(is_misplaced);
        battleship::paint(console_view, placed_battleship, battleship_style, pixel_size);
    }

    if (selected_battleship)
    {
        battleship::paint(console_view, *selected_battleship, constants::style::battleship::selected_style, pixel_size);
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
    if (!selected_battleship)
    {
        if (on_cancel_placement)
        {
            on_cancel_placement();
        }
        return false;
    }

    if (key == console::keyboard::ENTER)
    {
        if (on_submit_placement)
        {
            on_submit_placement(*selected_battleship);
        }
        return true;
    }

    if (key == console::keyboard::BACKSPACE)
    {
        if (on_reset_placement)
        {
            on_reset_placement(*selected_battleship);
        }
        return true;
    }

    if (key == console::keyboard::ESCAPE)
    {
        if (on_cancel_placement)
        {
            on_cancel_placement();
        }
        return true;
    }

    models::battleship updated_battleship = *selected_battleship;

    if (key == console::keyboard::SPACE || key == console::keyboard::R)
    {
        const models::battleship rotated_battleship = updated_battleship.rotated();
        updated_battleship = rotated_battleship;
    }

    if (key.is_arrow())
    {
        const models::offset battleship_position_offset = key.get_arrow_offset();
        const models::battleship moved_battleship = updated_battleship.moved_by(battleship_position_offset);
        updated_battleship = moved_battleship;
    }

    // Ensure the selected battleship stays on the board
    updated_battleship.rectangle = updated_battleship.rectangle.fitted_into(board_rectangle);
    selected_battleship = updated_battleship;

    invalidate();
    return true;
}

void components::board_designer::set_selected_battleship(const std::optional<models::battleship>& battleship_to_select)
{
    selected_battleship = battleship_to_select;
    invalidate();
}
