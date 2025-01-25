#pragma once

namespace models
{
    struct size
    {
        int width = 0, height = 0;

        bool operator==(const size& other_size) const;
    };
}
