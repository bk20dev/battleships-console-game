#include "battleship.hpp"

void components::battleship::paint(
    const std::shared_ptr<const console::console>& console, const models::battleship& battleship_to_paint,
    const console::style::style& style, const core::size& pixel_size)
{
    const core::rectangle battleship_fill_rectangle = battleship_to_paint.rectangle.scaled_by(pixel_size);
    const std::string battleship_style_sequence = style.to_control_sequence();
    console->fill_rectangle(battleship_fill_rectangle, solid_fill_character, battleship_style_sequence);
}
