#include "position.hpp"

#include <__algorithm/clamp.h>

#include "rectangle.hpp"

models::position models::position::fitted_into(const rectangle& target_rectangle) const
{
    const auto [target_x, target_y] = target_rectangle.position;
    const auto [target_width, target_height] = target_rectangle.size;

    return position{
        .x = std::clamp(x, target_x, target_x + target_width - 1),
        .y = std::clamp(y, target_y, target_y + target_height - 1),
    };
}

models::position models::position::operator+(const offset& offset_to_add) const
{
    return position{
        .x = x + offset_to_add.x,
        .y = y + offset_to_add.y,
    };
}

bool models::position::operator==(const position& other_position) const
{
    return x == other_position.x && y == other_position.y;
}
