#include "opponent_board.hpp"

#include "../constants/dimension.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};

    constexpr core::rectangle board_rectangle = {
        .size = {
            .width = constants::dimension::board_column_count,
            .height = constants::dimension::board_row_count,
        },
    };

    constexpr int total_board_width = constants::dimension::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::dimension::board_row_count * pixel_size.height;
}

namespace
{
    constexpr console::style::style selection_style = {
        .foreground_color = console::style::color::WHITE,
        .background_color = console::style::color::CYAN,
    };

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

void components::opponent_board::paint_crosshair(const core::rectangle& clip_rectangle,
                                                 const std::string& fill_character,
                                                 const console::style::style& crosshair_style) const
{
    // Paint horizontal crosshair line
    const core::rectangle horizontal_crosshair_rectangle = {
        .position = {.y = crosshair_position.y},
        .size = {.width = constants::dimension::board_column_count, .height = 1},
    };
    const core::rectangle clipped_horizontal_crosshair_rectangle =
        horizontal_crosshair_rectangle.intersect(clip_rectangle);
    paint_scaled_rectangle(console_view, clipped_horizontal_crosshair_rectangle,
                           fill_character, crosshair_style, pixel_size);

    // Paint vertical crosshair line
    const core::rectangle vertical_crosshair_rectangle = {
        .position = {.x = crosshair_position.x},
        .size = {.width = 1, .height = constants::dimension::board_row_count},
    };
    const core::rectangle clipped_vertical_crosshair_rectangle =
        vertical_crosshair_rectangle.intersect(clip_rectangle);
    paint_scaled_rectangle(console_view, clipped_vertical_crosshair_rectangle,
                           fill_character, crosshair_style, pixel_size);
}

void components::opponent_board::paint_crosshair_if_focused(
    const core::rectangle& clip_rectangle, const std::string& fill_character,
    const console::style::color& foreground_color) const
{
    if (!is_focused)
    {
        return;
    }

    console::style::style combined_selection_style = selection_style;
    combined_selection_style.foreground_color = foreground_color;

    paint_crosshair(clip_rectangle, fill_character, combined_selection_style);
}

void components::opponent_board::paint_board() const
{
    paint_scaled_rectangle(console_view, board_rectangle, constants::style::tertiary_fill_character,
                           constants::style::board::default_style, pixel_size);
    paint_crosshair_if_focused(board_rectangle, constants::style::tertiary_fill_character,
                               console::style::color::BRIGHT_BLACK);
}

void components::opponent_board::paint_prohibited_areas() const
{
    for (const auto& destroyed_battleship : destroyed_battleships)
    {
        const core::rectangle prohibited_area_rectangle = destroyed_battleship.rectangle.expanded_by(1);
        paint_scaled_rectangle(console_view, prohibited_area_rectangle, constants::style::tertiary_fill_character,
                               constants::style::battleship::destroyed_style, pixel_size);
        paint_crosshair_if_focused(prohibited_area_rectangle, constants::style::tertiary_fill_character);
    }
}

bool components::opponent_board::is_battleship_hit(const core::position& position) const
{
    return std::ranges::any_of(destroyed_battleships, [position](const auto& destroyed_battleship)
    {
        return destroyed_battleship.rectangle.intersects(position);
    }) || std::ranges::any_of(revealed_battleship_parts, [position](const auto& revealed_battleship_part)
    {
        return revealed_battleship_part == position;
    });
}

void components::opponent_board::paint_current_player_bullet(const models::bullet& bullet) const
{
    std::string bullet_fill_character = constants::style::tertiary_fill_character;
    console::style::style bullet_style = missed_bullet_style;

    if (is_battleship_hit(bullet.position))
    {
        bullet_fill_character = constants::style::primary_fill_character;
        bullet_style = constants::style::battleship::destroyed_style;
    }
    if (is_focused && (bullet.position.x == crosshair_position.x || bullet.position.y == crosshair_position.y))
    {
        bullet_style = selection_style;
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

bool components::opponent_board::is_any_object_hit(const core::position& position) const
{
    if (is_battleship_hit(position))
    {
        return true;
    }
    if (std::ranges::any_of(current_player_bullets, [position](const models::bullet& current_player_bullet)
    {
        return current_player_bullet.position == position;
    }))
    {
        return true;
    }
    return false;
}

components::opponent_board::opponent_board(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<models::bullet>& current_player_bullets,
    const std::vector<core::position>& revealed_battleship_parts,
    const std::vector<models::battleship>& destroyed_battleships,
    const std::function<void(const core::position& position)>& on_position_select)
    : component(x, y, total_board_width, total_board_height, console),
      current_player_bullets(current_player_bullets),
      revealed_battleship_parts(revealed_battleship_parts),
      destroyed_battleships(destroyed_battleships),
      on_position_select(on_position_select)
{
}

void components::opponent_board::paint()
{
    paint_board();
    paint_prohibited_areas();
    paint_current_player_bullets();

    component::paint();
}

bool components::opponent_board::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key.is_arrow())
    {
        const core::offset key_arrow_offset = key.get_arrow_offset();
        const core::position updated_crosshair_position = crosshair_position + key_arrow_offset;
        crosshair_position = updated_crosshair_position.fitted_into(board_rectangle);

        invalidate();
        return true;
    }

    if (key == console::keyboard::character::ENTER)
    {
        if (on_position_select && !is_any_object_hit(crosshair_position))
        {
            on_position_select(crosshair_position);
        }
        return true;
    }

    return component::handle_keyboard_event(key);
}

void components::opponent_board::focus()
{
    invalidate();
    focusable::focus();
}

void components::opponent_board::blur()
{
    invalidate();
    focusable::blur();
}
