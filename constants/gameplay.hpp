#pragma once

#include <array>

#include "../models/battleship.hpp"

namespace constants::gameplay
{
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
