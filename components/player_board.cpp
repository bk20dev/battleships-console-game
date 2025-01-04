#include "player_board.hpp"

#include "../console/style.hpp"
#include "../constants/constants.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};

    constexpr int total_board_width = constants::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::board_row_count * pixel_size.height;
}

namespace
{
    constexpr console::style::style board_style = {
        .foreground_color = console::style::BRIGHT_BLACK,
    };
}

void components::player_board::paint_board() const
{
    static constexpr core::rectangle board_fill_rectangle = {
        .size = {
            .width = total_board_width,
            .height = total_board_height,
        },
    };

    const std::string board_style_sequence = board_style.to_control_sequence();
    console_view->fill_rectangle(board_fill_rectangle, constants::tertiary_fill_character, board_style_sequence);
}

components::player_board::player_board(const int x, const int y,
                                       const std::shared_ptr<console::console>& console): component(
    x, y, total_board_width, total_board_height, console)
{
}

void components::player_board::paint()
{
    paint_board();

    component::paint();
}
