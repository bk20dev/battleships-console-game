#include "size.hpp"

bool models::size::operator==(const size& other_size) const
{
    return width == other_size.width && height == other_size.height;
}
