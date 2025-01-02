#pragma once

#include <string>

#include "../console/console.hpp"
#include "../console/style.hpp"
#include "../models/battleship.hpp"

namespace components::battleship
{
    constexpr std::string solid_fill_character = "\u2593";

    constexpr console::style::style default_style = {
        .foreground_color = console::style::WHITE,
    };

    constexpr console::style::style selected_style = {
        .foreground_color = console::style::CYAN,
    };

    constexpr console::style::style misplaced_style = {
        .foreground_color = console::style::RED,
    };

    void paint(const std::shared_ptr<const console::console>& console, const models::battleship& battleship_to_paint,
               const console::style::style& style, const core::size& pixel_size = {1, 1});
}
