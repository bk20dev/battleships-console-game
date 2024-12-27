#include "dimension.hpp"

core::position core::position::operator+(const offset& offset_to_add) const
{
    return position{
        .x = x + offset_to_add.x,
        .y = y + offset_to_add.y,
    };
}
