#pragma once

#include "../models/battleship.hpp"

namespace constants
{
    constexpr int board_row_count = 10;
    constexpr int board_column_count = 10;

    /** Used for painting primary visual elements, e.g. battleships. */
    constexpr std::string primary_fill_character = "\u2593";
    /** Used for painting additional visual hints. */
    constexpr std::string secondary_fill_character = "\u2592";
    /** Used for painting backgrounds. */
    constexpr std::string tertiary_fill_character = "\u2591";

    /** A set of predefined battleships. */
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
