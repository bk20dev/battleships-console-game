#pragma once

#include "offset.hpp"

namespace models
{
    struct rectangle;

    struct position
    {
        int x = 0, y = 0;

        [[nodiscard]] position fitted_into(const rectangle& target_rectangle) const;

        position operator+(const offset& offset_to_add) const;
        bool operator==(const position& other_position) const;
    };
}
