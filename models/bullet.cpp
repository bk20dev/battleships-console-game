#include "bullet.hpp"

core::rectangle models::bullet::get_rectangle() const
{
    return core::rectangle{
        .position = position,
        .size = {.width = 1, .height = 1},
    };
}
