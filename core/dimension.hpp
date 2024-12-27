#pragma once

namespace core
{
    struct offset
    {
        int x = 0, y = 0;
    };

    struct position
    {
        int x = 0, y = 0;

        position operator+(const offset& offset_to_add) const;
    };

    struct size
    {
        int width, height;
    };
}
