#include "battleship.hpp"

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
    const console::style::style& style, const core::size& pixel_size)
{
    const core::rectangle battleship_fill_rectangle = scale_rectangle(battleship_to_paint.rectangle, pixel_size);
    const std::string battleship_style_sequence = style.to_control_sequence();
    console->fill_rectangle(battleship_fill_rectangle, solid_fill_character, battleship_style_sequence);
}
