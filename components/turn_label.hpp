#pragma once

#include "label.hpp"

namespace components
{
    class turn_label final : label
    {
        int label_width;

        bool is_background_displayed = false;
        bool player_won = false;

        void paint_won_background() const;
        void paint_lost_background() const;
        void paint_background() const;

    public:
        turn_label(int x, int y, int width, const std::shared_ptr<console::console>& console);

        void set_current_player_turn();
        void set_opponent_player_turn();

        void set_current_player_won();
        void set_current_player_lost();

        void paint() override;
    };
}
