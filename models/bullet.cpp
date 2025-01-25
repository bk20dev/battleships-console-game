#include "bullet.hpp"

#include "rectangle.hpp"

models::rectangle models::bullet::get_rectangle() const
{
    return rectangle{
        .position = position,
        .size = {.width = 1, .height = 1},
    };
}

bool models::bullet::operator==(const bullet& other_bullet) const
{
    return position == other_bullet.position;
}
