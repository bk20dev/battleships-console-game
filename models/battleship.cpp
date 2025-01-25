#include "battleship.hpp"

models::battleship models::battleship::rotated() const
{
    return with_rectangle(rectangle.rotated());
}

models::battleship models::battleship::moved_by(const offset& offset) const
{
    return with_rectangle(rectangle + offset);
}

models::battleship models::battleship::normalized() const
{
    return with_rectangle(rectangle.normalized());
}

bool models::battleship::operator==(const battleship& other_battleship) const
{
    return id == other_battleship.id && rectangle == other_battleship.rectangle;
}

models::battleship models::battleship::with_rectangle(const models::rectangle& updated_rectangle) const
{
    battleship updated_battleship = *this;
    updated_battleship.rectangle = updated_rectangle;
    return updated_battleship;
}
