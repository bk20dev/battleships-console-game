#pragma once

namespace core
{
    struct position
    {
        int x = 0, y = 0;

        position operator+(const position& other_position) const;
    };

    struct offset
    {
        int x = 0, y = 0;
    };

    struct size
    {
        int width, height;
    };

    inline position position::operator+(const position& other_position) const
    {
        return position{
            .x = x + other_position.x,
            .y = y + other_position.y
        };
    }
}
