#pragma once
#include "rectangle.hpp"

namespace models
{
    struct battleship
    {
        int id = -1;
        rectangle rectangle;

        [[nodiscard]] battleship rotated() const;

        [[nodiscard]] battleship moved_by(const offset& offset) const;

        [[nodiscard]] battleship normalized() const;

        bool operator==(const battleship& other_battleship) const;

    private:
        [[nodiscard]] battleship with_rectangle(const models::rectangle& updated_rectangle) const;
    };
}
