#pragma once

#include "../console/style.hpp"

namespace core::component_traits
{
    class styleable
    {
    protected:
        console::style::style style;

        ~styleable() = default;

    public:
        virtual void set_style(const console::style::style& new_style);
    };

    class focusable
    {
    protected:
        bool is_focused = false;

        ~focusable() = default;

    public:
        virtual void focus();

        virtual void blur();

        [[nodiscard]] bool get_is_focused() const;
    };

    class activatable
    {
    protected:
        bool is_enabled = true;

        ~activatable() = default;

    public:
        virtual void enable();

        virtual void disable();

        [[nodiscard]] bool get_is_enabled() const;
    };
}
