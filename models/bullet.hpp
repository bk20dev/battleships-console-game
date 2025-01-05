#pragma once

#include "../core/dimension.hpp"

namespace models
{
    struct bullet
    {
        core::position position;

        [[nodiscard]] core::rectangle get_rectangle() const;

        bool operator==(const bullet& other_bullet) const;
    };
}
