#pragma once

#include "../console/console.hpp"
#include "../console/style.hpp"
#include "../models/battleship.hpp"

namespace components::battleship
{
    void paint(const std::shared_ptr<const console::console>& console, const models::battleship& battleship_to_paint,
               const console::style::style& style, const models::size& pixel_size = {1, 1});
}
