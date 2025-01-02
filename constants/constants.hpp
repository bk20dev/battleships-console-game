#pragma once

namespace constants
{
    constexpr int board_row_count = 10;
    constexpr int board_column_count = 10;

    constexpr std::array battleships = {
        models::battleship{.id = 0, .rectangle = {.size = {.height = 1, .width = 4}}},
        models::battleship{.id = 1, .rectangle = {.size = {.height = 1, .width = 3}}},
        models::battleship{.id = 2, .rectangle = {.size = {.height = 1, .width = 3}}},
        models::battleship{.id = 3, .rectangle = {.size = {.height = 1, .width = 2}}},
        models::battleship{.id = 4, .rectangle = {.size = {.height = 1, .width = 2}}},
        models::battleship{.id = 5, .rectangle = {.size = {.height = 1, .width = 2}}},
        models::battleship{.id = 6, .rectangle = {.size = {.height = 1, .width = 1}}},
        models::battleship{.id = 7, .rectangle = {.size = {.height = 1, .width = 1}}},
        models::battleship{.id = 8, .rectangle = {.size = {.height = 1, .width = 1}}},
        models::battleship{.id = 9, .rectangle = {.size = {.height = 1, .width = 1}}},
    };
}
