#pragma once

#include "../core/dimension.hpp"

namespace models
{
    struct battleship
    {
        int id = -1;
        core::rectangle rectangle;

        battleship rotated() const
        {
            return {
                .id = id,
                .rectangle = rectangle.rotated(),
            };
        }

        battleship moved_by(const core::offset& offset) const
        {
            return {
                .id = id,
                .rectangle = rectangle + offset,
            };
        }

        battleship normalized() const
        {
            return {
                .id = id,
                .rectangle = rectangle.normalized(),
            };
        }

        bool operator==(const battleship& other_battleship) const
        {
            return id == other_battleship.id && rectangle == other_battleship.rectangle;
        }
    };
}
