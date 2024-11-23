#pragma once

#include "../console/style.hpp"

namespace core::component_traits
{
    class styleable
    {
    protected:
        console::style::style style;

    public:
        void set_style(const console::style::style& new_style);
    };

    class focusable
    {
    protected:
        bool is_focused = false;

    public:
        void focus();

        void blur();
    };

    class activatable
    {
    protected:
        bool is_enabled = true;

    public:
        void enable();

        void disable();
    };
}
