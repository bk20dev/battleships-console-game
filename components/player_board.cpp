#include "player_board.hpp"

#include "battleship.hpp"
#include "../console/style.hpp"
#include "../constants/dimension.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr models::size pixel_size = {.width = 2, .height = 1};

    constexpr int total_board_width = constants::dimension::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::dimension::board_row_count * pixel_size.height;
}

namespace
{
    constexpr console::style::style missed_bullet_style = {
        .foreground_color = console::style::BLUE,
    };
}

static void paint_scaled_rectangle(const std::shared_ptr<const console::console>& console,
                                   const models::rectangle& rectangle, const std::string& character,
                                   const console::style::style& style, const models::size& pixel_size = ::pixel_size)
{
    const models::rectangle scaled_fill_rectangle = rectangle.scaled_by(pixel_size);
    const std::string style_sequence = style.to_control_sequence();
    console->fill_rectangle(scaled_fill_rectangle, character, style_sequence);
}

void components::player_board::paint_board() const
{
    static constexpr models::rectangle board_fill_rectangle = {
        .size = {
            .width = total_board_width,
            .height = total_board_height,
        },
    };

    const std::string board_style_sequence = constants::style::board::default_style.to_control_sequence();
    console_view->fill_rectangle(board_fill_rectangle, constants::style::tertiary_fill_character, board_style_sequence);
}

void components::player_board::paint_placed_battleships() const
{
    for (const auto& placed_battleship : placed_battleships)
    {
        battleship::paint(console_view, placed_battleship, constants::style::battleship::default_style, pixel_size);
    }
}

bool components::player_board::is_battleship_hit(const models::bullet& bullet) const
{
    return std::ranges::any_of(placed_battleships, [bullet](const auto& placed_battleship)
    {
        return placed_battleship.rectangle.intersects(bullet.position);
    });
}

void components::player_board::paint_opponent_bullet(const models::bullet& bullet) const
{
    std::string bullet_fill_character = constants::style::tertiary_fill_character;
    console::style::style bullet_style = missed_bullet_style;

    if (is_battleship_hit(bullet))
    {
        bullet_fill_character = constants::style::primary_fill_character;
        bullet_style = constants::style::battleship::destroyed_style;
    }

    paint_scaled_rectangle(console_view, bullet.get_rectangle(), bullet_fill_character, bullet_style, pixel_size);
}

void components::player_board::paint_opponent_bullets() const
{
    for (const auto& opponent_bullet : opponent_bullets)
    {
        paint_opponent_bullet(opponent_bullet);
    }
}

components::player_board::player_board(const int x, const int y, const std::shared_ptr<console::console>& console,
                                       const std::vector<models::battleship>& placed_battleships,
                                       const std::vector<models::bullet>& opponent_bullets)
    : component(x, y, total_board_width, total_board_height, console),
      placed_battleships(placed_battleships), opponent_bullets(opponent_bullets)
{
}

void components::player_board::paint()
{
    paint_board();
    paint_placed_battleships();
    paint_opponent_bullets();

    component::paint();
}
