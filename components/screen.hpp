#pragma once

#include "keyboard_actions.hpp"
#include "label.hpp"
#include "../constants/dimension.hpp"
#include "../core/component.hpp"

namespace components
{
    class screen : public core::component
    {
        std::shared_ptr<label> screen_title_label;
        std::shared_ptr<keyboard_actions::footer_component> keyboard_actions_footer;

        void initialize_screen_title_label(const std::string& screen_title);
        void initialize_keyboard_actions_footer();

        void initialize_child_console_view();

        void paint_components() const;

    protected:
        std::shared_ptr<console::console> child_console_view;

        void set_keyboard_actions(const std::vector<keyboard_actions::keyboard_action>& keyboard_actions) const;

    public:
        static constexpr models::offset child_console_view_offset = {.x = 0, .y = 5};
        static constexpr int screen_footer_height = 1;

        static constexpr int child_console_view_height = constants::dimension::screen_height
            - child_console_view_offset.y - screen_footer_height;

        screen(int x, int y, const std::shared_ptr<console::console>& console, const std::string& screen_title);

        void paint() override;
    };
}
