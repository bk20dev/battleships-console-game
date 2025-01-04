#include "opponent_board.hpp"

#include "../constants/dimension.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};

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
                                   const core::rectangle& rectangle, const std::string& character,
                                   const console::style::style& style, const core::size& pixel_size = ::pixel_size)
{
    const core::rectangle scaled_fill_rectangle = rectangle.scaled_by(pixel_size);
    const std::string style_sequence = style.to_control_sequence();
    console->fill_rectangle(scaled_fill_rectangle, character, style_sequence);
}

void components::opponent_board::paint_board() const
{
    static constexpr core::rectangle board_fill_rectangle = {
        .size = {
            .width = total_board_width,
            .height = total_board_height,
        },
    };

    const std::string board_style_sequence = constants::style::board::default_style.to_control_sequence();
    console_view->fill_rectangle(board_fill_rectangle, constants::style::tertiary_fill_character, board_style_sequence);
}

void components::opponent_board::paint_prohibited_areas() const
{
    for (const auto& destroyed_battleship : destroyed_battleships)
    {
        const core::rectangle prohibited_area_rectangle = destroyed_battleship.rectangle.expanded_by(1);
        paint_scaled_rectangle(console_view, prohibited_area_rectangle, constants::style::tertiary_fill_character,
                               constants::style::battleship::destroyed_style, pixel_size);
    }
}

bool components::opponent_board::is_battleship_hit(const models::bullet& bullet) const
{
    return std::ranges::any_of(destroyed_battleships, [bullet](const auto& destroyed_battleship)
    {
        return destroyed_battleship.rectangle.intersects(bullet.position);
    }) || std::ranges::any_of(revealed_battleship_parts, [bullet](const auto& revealed_battleship_part)
    {
        return revealed_battleship_part == bullet.position;
    });
}

void components::opponent_board::paint_current_player_bullet(const models::bullet& bullet) const
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

void components::opponent_board::paint_current_player_bullets() const
{
    for (const auto& current_player_bullet : current_player_bullets)
    {
        paint_current_player_bullet(current_player_bullet);
    }
}

components::opponent_board::opponent_board(const int x, const int y, const std::shared_ptr<console::console>& console,
                                           const std::vector<models::bullet>& current_player_bullets,
                                           const std::vector<core::position>& revealed_battleship_parts,
                                           const std::vector<models::battleship>& destroyed_battleships)
    : component(x, y, total_board_width, total_board_height, console),
      current_player_bullets(current_player_bullets),
      revealed_battleship_parts(revealed_battleship_parts),
      destroyed_battleships(destroyed_battleships)
{
}

void components::opponent_board::paint()
{
    paint_board();
    paint_prohibited_areas();
    paint_current_player_bullets();

    component::paint();
}
