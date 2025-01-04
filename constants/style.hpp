#pragma once

#include <string>

namespace constants::style
{
    /** Used for painting primary visual elements, e.g. battleships. */
    constexpr std::string primary_fill_character = "\u2593";
    /** Used for painting additional visual hints. */
    constexpr std::string secondary_fill_character = "\u2592";
    /** Used for painting backgrounds. */
    constexpr std::string tertiary_fill_character = "\u2591";
}
