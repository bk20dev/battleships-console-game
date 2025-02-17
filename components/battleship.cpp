#include "battleship.hpp"

#include "../constants/style.hpp"

void components::battleship::paint(
    const std::shared_ptr<const console::console>& console, const models::battleship& battleship_to_paint,
    const console::style::style& style, const models::size& pixel_size)
{
    const models::rectangle battleship_fill_rectangle = battleship_to_paint.rectangle.scaled_by(pixel_size);
    const std::string battleship_fill_character = constants::style::primary_fill_character;
    const std::string battleship_style_sequence = style.to_control_sequence();

    console->fill_rectangle(battleship_fill_rectangle, battleship_fill_character, battleship_style_sequence);
}
