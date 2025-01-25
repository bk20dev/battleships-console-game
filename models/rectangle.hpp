#pragma once

#include "offset.hpp"
#include "position.hpp"
#include "size.hpp"

namespace models
{
    struct rectangle
    {
        position position;
        size size;

        [[nodiscard]] int get_area() const;

        [[nodiscard]] rectangle fitted_into(const rectangle& target_rectangle) const;

        [[nodiscard]] rectangle rotated() const;

        [[nodiscard]] rectangle normalized() const;

        [[nodiscard]] rectangle expanded_by(int amount) const;

        [[nodiscard]] rectangle scaled_by(const models::size& pixel_size) const;

        [[nodiscard]] rectangle intersect(const rectangle& other_rectangle) const;

        [[nodiscard]] bool intersects(const models::position& other_position) const;

        [[nodiscard]] bool intersects(const rectangle& other_rectangle) const;

        rectangle operator+(const offset& offset_to_add) const;

        bool operator==(const rectangle& other_rectangle) const;
    };
}
