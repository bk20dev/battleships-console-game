#include "rectangle.hpp"

#include <algorithm>
#include <stdexcept>
#include <__algorithm/clamp.h>

#include "size.hpp"

int fit_line_segment(const int segment_start, const int segment_length,
                     const int target_segment_start, const int target_segment_length)
{
    const int target_segment_end = target_segment_start + target_segment_length;
    return std::clamp(segment_start, target_segment_start, target_segment_end - segment_length);
}

int models::rectangle::get_area() const
{
    return size.width * size.height;
}

models::rectangle models::rectangle::fitted_into(const rectangle& target_rectangle) const
{
    const auto [x, y] = position;
    const auto [width, height] = size;
    const auto [target_x, target_y] = target_rectangle.position;
    const auto [target_width, target_height] = target_rectangle.size;

    if (width > target_width || height > target_height)
    {
        throw std::range_error("The target rectangle is too small.");
    }

    const models::position result_position = {
        .x = fit_line_segment(x, width, target_x, target_width),
        .y = fit_line_segment(y, height, target_y, target_height),
    };

    const rectangle result_rectangle = {
        .position = result_position,
        .size = size,
    };
    return result_rectangle;
}

models::rectangle models::rectangle::rotated() const
{
    const models::size result_size = {
        .width = size.height,
        .height = size.width,
    };

    return {
        .position = position,
        .size = result_size,
    };
}

models::rectangle models::rectangle::normalized() const
{
    if (size.width < size.height)
    {
        return rotated();
    }
    return *this;
}

models::rectangle models::rectangle::expanded_by(const int amount) const
{
    return {
        .position = {
            .x = position.x - amount,
            .y = position.y - amount,
        },
        .size = {
            .width = amount + size.width + amount,
            .height = amount + size.height + amount,
        },
    };
}

models::rectangle models::rectangle::scaled_by(const models::size& pixel_size) const
{
    return rectangle{
        .position = {
            .x = position.x * pixel_size.width,
            .y = position.y * pixel_size.height,
        },
        .size = {
            .width = size.width * pixel_size.width,
            .height = size.height * pixel_size.height,
        }
    };
}

models::rectangle models::rectangle::intersect(const rectangle& other_rectangle) const
{
    const int x = std::max(position.x, other_rectangle.position.x);
    const int y = std::max(position.y, other_rectangle.position.y);

    const int tx = std::min(position.x + size.width, other_rectangle.position.x + other_rectangle.size.width);
    const int ty = std::min(position.y + size.height, other_rectangle.position.y + other_rectangle.size.height);

    const int width = std::max(0, tx - x);
    const int height = std::max(0, ty - y);

    return rectangle{
        .position = {x, y},
        .size = {width, height},
    };
}

bool is_point_between(const int point, const int a, const int b)
{
    return point >= a && point <= b;
}

bool models::rectangle::intersects(const models::position& other_position) const
{
    const int ax = position.x, bx = ax + size.width - 1;
    const int ay = position.y, by = ay + size.height - 1;
    return is_point_between(other_position.x, ax, bx) && is_point_between(other_position.y, ay, by);
}

bool is_segment_between(const int a, const int b, const int p, const int q)
{
    return is_point_between(a, p, q) || is_point_between(b, p, q);
}

bool is_any_segment_between(const int a, const int b, const int p, const int q)
{
    return is_segment_between(a, b, p, q) || is_segment_between(p, q, a, b);
}

bool models::rectangle::intersects(const rectangle& other_rectangle) const
{
    const int ax = position.x, bx = ax + size.width - 1,
              px = other_rectangle.position.x, qx = px + other_rectangle.size.width - 1;
    const int ay = position.y, by = ay + size.height - 1,
              py = other_rectangle.position.y, qy = py + other_rectangle.size.height - 1;

    return is_any_segment_between(ax, bx, px, qx) && is_any_segment_between(ay, by, py, qy);
}

models::rectangle models::rectangle::operator+(const offset& offset_to_add) const
{
    return {
        .position = position + offset_to_add,
        .size = size,
    };
}

bool models::rectangle::operator==(const rectangle& other_rectangle) const
{
    return position == other_rectangle.position && size == other_rectangle.size;
}
