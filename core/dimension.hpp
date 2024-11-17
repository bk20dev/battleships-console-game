#pragma once

namespace core
{
    struct position
    {
        int x, y;

        position operator+(const position& other_position) const;
    };

    inline position position::operator+(const position& other_position) const
    {
        return position{
            .x = x + other_position.x,
            .y = y + other_position.y
        };
    }

    struct size
    {
        int width, height;
    };
}
