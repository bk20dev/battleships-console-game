#pragma once

#include "position.hpp"

namespace models
{
    struct bullet
    {
        position position;

        [[nodiscard]] rectangle get_rectangle() const;

        bool operator==(const bullet& other_bullet) const;
    };
}
