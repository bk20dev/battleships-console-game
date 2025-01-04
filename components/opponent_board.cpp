#include "opponent_board.hpp"

#include "../constants/dimension.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};

    constexpr int total_board_width = constants::dimension::board_column_count * pixel_size.width;
    constexpr int total_board_height = constants::dimension::board_row_count * pixel_size.height;
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

components::opponent_board::opponent_board(int x, int y, const std::shared_ptr<console::console>& console)
    : component(x, y, total_board_width, total_board_height, console)
{
}

void components::opponent_board::paint()
{
    paint_board();

    component::paint();
}
