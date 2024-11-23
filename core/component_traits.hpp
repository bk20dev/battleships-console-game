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
}
