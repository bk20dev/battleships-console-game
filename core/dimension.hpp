#pragma once

namespace core
{
    struct rectangle;

    struct offset
    {
        int x = 0, y = 0;
    };

    struct position
    {
        int x = 0, y = 0;

        [[nodiscard]] position fitted_into(const rectangle& target_rectangle) const;

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

        [[nodiscard]] int get_area() const;

        [[nodiscard]] rectangle fitted_into(const rectangle& target_rectangle) const;

        [[nodiscard]] rectangle rotated() const;

        [[nodiscard]] rectangle normalized() const;

        [[nodiscard]] rectangle expanded_by(int amount) const;

        [[nodiscard]] rectangle scaled_by(const core::size& pixel_size) const;

        [[nodiscard]] rectangle intersect(const rectangle& other_rectangle) const;

        [[nodiscard]] bool intersects(const core::position& other_position) const;

        [[nodiscard]] bool intersects(const rectangle& other_rectangle) const;

        rectangle operator+(const offset& offset_to_add) const;

        bool operator==(const rectangle& other_rectangle) const;
    };
}
