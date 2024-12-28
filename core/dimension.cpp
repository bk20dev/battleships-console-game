#include "dimension.hpp"

#include <stdexcept>
#include <__algorithm/clamp.h>

core::position core::position::operator+(const offset& offset_to_add) const
{
    return position{
        .x = x + offset_to_add.x,
        .y = y + offset_to_add.y,
    };
}

int fit_line_segment(const int segment_start, const int segment_length,
                     const int target_segment_start, const int target_segment_length)
{
    const int target_segment_end = target_segment_start + target_segment_length;
    return std::clamp(segment_start, target_segment_start, target_segment_end - segment_length);
}

core::rectangle core::rectangle::fitted_into(const rectangle& target_rectangle) const
{
    const auto [x, y] = position;
    const auto [width, height] = size;
    const auto [target_x, target_y] = target_rectangle.position;
    const auto [target_width, target_height] = target_rectangle.size;

    if (width > target_width || height > target_height)
    {
        throw new std::range_error("The target rectangle is too small.");
    }

    const core::position result_position = {
        .x = fit_line_segment(x, width, target_x, target_width),
        .y = fit_line_segment(y, height, target_y, target_height),
    };

    const rectangle result_rectangle = {
        .position = result_position,
        .size = size,
    };
    return result_rectangle;
}

core::rectangle core::rectangle::operator+(const offset& offset_to_add) const
{
    return {
        .position = position + offset_to_add,
        .size = size,
    };
}
