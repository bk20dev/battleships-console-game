#pragma once

#include "../core/dimension.hpp"

namespace models
{
    struct battleship
    {
        int id = -1;
        core::rectangle rectangle;

        [[nodiscard]] battleship rotated() const;

        [[nodiscard]] battleship moved_by(const core::offset& offset) const;

        [[nodiscard]] battleship normalized() const;

        bool operator==(const battleship& other_battleship) const;

    private:
        [[nodiscard]] battleship with_rectangle(const core::rectangle& updated_rectangle) const;
    };
}
