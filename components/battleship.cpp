#include "battleship.hpp"

static console::style::style get_battleship_style(const bool is_selected, const bool is_misplaced)
{
    if (is_selected && is_misplaced)
    {
        return components::battleship::selected_misplaced_style;
    }
    if (is_selected)
    {
        return components::battleship::selected_style;
    }
    if (is_misplaced)
    {
        return components::battleship::misplaced_style;
    }
    return components::battleship::default_style;
}

static core::rectangle scale_rectangle(const core::rectangle& rectangle, const core::size& pixel_size)
{
    const auto [position, size] = rectangle;

    return core::rectangle{
        .position = {
            .x = position.x * pixel_size.width,
            .y = position.y * pixel_size.height,
        },
        .size = {
            .width = size.width * pixel_size.width,
            .height = size.height * pixel_size.height,
        }
    };
}

void components::battleship::paint(
    const std::shared_ptr<const console::console>& console, const models::battleship& battleship_to_paint,
    const bool is_selected, const bool is_misplaced, const core::size& pixel_size)
{
    const core::rectangle battleship_fill_rectangle = scale_rectangle(battleship_to_paint.rectangle, pixel_size);

    const console::style::style battleship_style = get_battleship_style(is_selected, is_misplaced);
    const std::string battleship_style_sequence = battleship_style.to_control_sequence();

    console->fill_rectangle(battleship_fill_rectangle, solid_fill_character, battleship_style_sequence);
}
