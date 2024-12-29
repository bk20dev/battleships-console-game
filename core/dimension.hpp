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

        bool operator==(const position& other_position) const;
    };

    struct size
    {
        int width = 0, height = 0;

        bool operator==(const size& other_size) const;
    };

    struct rectangle
    {
        position position;
        size size;

        [[nodiscard]] rectangle fitted_into(const rectangle& target_rectangle) const;

        [[nodiscard]] rectangle rotated() const;

        rectangle operator+(const offset& offset_to_add) const;

        bool operator==(const rectangle& other_rectangle) const;
    };
}
