#include "component_traits.hpp"

void core::component_traits::styleable::set_style(const console::style::style& new_style)
{
    style = new_style;
}

void core::component_traits::focusable::focus()
{
    is_focused = true;
}

void core::component_traits::focusable::blur()
{
    is_focused = false;
}

bool core::component_traits::focusable::get_is_focused() const
{
    return is_focused;
}
