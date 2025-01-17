#pragma once

#include <string>

#include "../console/style.hpp"

namespace constants::style
{
    /** Used for painting primary visual elements, e.g. battleships. */
    constexpr std::string primary_fill_character = "\u2593";
    /** Used for painting additional visual hints. */
    constexpr std::string secondary_fill_character = "\u2592";
    /** Used for painting backgrounds. */
    constexpr std::string tertiary_fill_character = "\u2591";

    namespace general
    {
        /** Used to draw textual hints. */
        constexpr console::style::style hint_style = {
            .text_effect = console::style::text_effect::FAINT,
        };
        /** Used to draw textual errors, e.g. validation errors. */
        constexpr console::style::style error_style = {
            .foreground_color = console::style::RED,
        };
        /** Used to draw textual errors, e.g. validation errors. */
        constexpr console::style::style notice_style = {
            .foreground_color = console::style::CYAN,
        };

        /** Used to draw selected elements, e.g. buttons. */
        constexpr console::style::style selected_style = {
            .background_color = console::style::CYAN,
        };

        /** Used to draw selectable elements which are inactive at the moment, e.g. buttons. */
        constexpr console::style::style deselected_style = {
            .background_color = console::style::BRIGHT_BLACK,
        };
    }

    namespace board
    {
        /** Default style used to draw game board background. */
        constexpr console::style::style default_style = {
            .foreground_color = console::style::BRIGHT_BLACK,
        };
    }

    namespace battleship
    {
        /** Default style used to draw battleships. */
        constexpr console::style::style default_style = {
            .foreground_color = console::style::WHITE,
        };
        /** Used to draw selected (highlighted) battleships. */
        constexpr console::style::style selected_style = {
            .foreground_color = console::style::CYAN,
        };

        /** Used to mark correctly placed battleships. */
        static constexpr console::style::style placed_style = {
            .foreground_color = console::style::BRIGHT_BLACK,
        };
        /** Used to highlight incorrectly placed battleships (e.g. having overlapping positions) */
        constexpr console::style::style misplaced_style = {
            .foreground_color = console::style::RED,
        };

        /** Used to highlight destroyed battleships. */
        constexpr console::style::style destroyed_style = {
            .foreground_color = console::style::RED,
        };
    }
}
