#include "turn_label.hpp"

#include "../console/style.hpp"

namespace
{
    constexpr console::style::style current_player_turn_label_style = {
        .foreground_color = console::style::CYAN,
    };
    constexpr console::style::style opponent_player_turn_label_style = {
        .foreground_color = console::style::WHITE,
    };
    constexpr console::style::style current_player_won_label_style = {
        .background_color = console::style::BRIGHT_BLUE,
    };
    constexpr console::style::style current_player_lost_label_style = {
        .background_color = console::style::RED,
    };
}

void components::turn_label::paint_won_background() const
{
    console_view->rainbow_fill_row(0, 0, label_width);
}

void components::turn_label::paint_lost_background() const
{
    const models::rectangle fill_rectangle = {
        .size = {.width = label_width, .height = 1},
    };
    static constexpr console::style::style lost_background_style = {
        .background_color = console::style::RED,
    };
    const std::string lost_background_style_sequence = lost_background_style.to_control_sequence();
    console_view->fill_rectangle(fill_rectangle, " ", lost_background_style_sequence);
}

void components::turn_label::paint_background() const
{
    if (player_won)
    {
        paint_won_background();
    }
    else
    {
        paint_lost_background();
    }
}

components::turn_label::turn_label(const int x, const int y, const int width,
                                   const std::shared_ptr<console::console>& console)
    : label(x, y, console, "Waiting for turn..."), label_width(width)
{
}

void components::turn_label::set_current_player_turn()
{
    set_text("Your turn");
    set_style(current_player_turn_label_style);
}

void components::turn_label::set_opponent_player_turn()
{
    set_text("Opponent's turn");
    set_style(opponent_player_turn_label_style);
}

void components::turn_label::set_current_player_won()
{
    set_text("You won");
    set_style(current_player_won_label_style);
    is_background_displayed = true;
    player_won = true;
    invalidate();
}

void components::turn_label::set_current_player_lost()
{
    set_text("You lost");
    set_style(current_player_lost_label_style);
    is_background_displayed = true;
    player_won = false;
    invalidate();
}

void components::turn_label::paint()
{
    if (is_background_displayed)
    {
        paint_background();
        paint_without_clearing_trail();
    }
    else
    {
        label::paint();
    }
}
