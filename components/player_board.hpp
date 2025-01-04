#pragma once
#include "../core/component.hpp"

namespace components
{
    class player_board final : public core::component
    {
        void paint_board() const;

    public:
        player_board(int x, int y, const std::shared_ptr<console::console>& console);

        void paint() override;
    };
}
