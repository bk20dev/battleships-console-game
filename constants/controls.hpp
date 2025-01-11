#pragma once

#include <array>
#include "../console/keyboard.hpp"

namespace constants::controls
{
    constexpr std::array select_keys = {
        console::keyboard::character::ENTER,
        console::keyboard::character::SPACE,
    };
}
