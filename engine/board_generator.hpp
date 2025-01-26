#pragma once

#include <vector>

#include "../constants/dimension.hpp"
#include "../models/battleship.hpp"

namespace engine::generator
{
    static constexpr models::size default_board_size = {
        .width = constants::dimension::board_column_count,
        .height = constants::dimension::board_row_count
    };

    [[nodiscard]] std::vector<models::battleship> random_board(
        std::vector<models::battleship> available_battleships,
        const models::size& board_size = default_board_size);
}
