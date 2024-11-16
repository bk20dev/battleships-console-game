#include "component.hpp"

void core::component::invalidate()
{
    needs_repaint = true;
}

bool core::component::should_repaint() const
{
    return needs_repaint;
}

void core::component::paint()
{
    needs_repaint = false;
}

bool core::component::handle_keyboard_event(const console::keyboard::key& key)
{
    return false;
}

core::component::component(const int x, const int y, const int width, const int height,
                           const std::shared_ptr<console::console_view>& console_view)
    : position{x, y}, size{width, height}, console(console_view)
{
}

void core::container_component::add_component(const std::shared_ptr<component>& other_component)
{
    children_components.emplace_back(other_component);
    other_component->parent = shared_from_this();
}

void core::container_component::paint()
{
    for (const auto& child_component : children_components)
    {
        child_component->paint();
    }
    component::paint();
}

bool core::container_component::handle_keyboard_event(const console::keyboard::key& key)
{
    for (const auto& child_component : children_components)
    {
        if (child_component->handle_keyboard_event(key))
        {
            if (child_component->should_repaint())
            {
                child_component->paint();
            }
            return true;
        }
    }
    return component::handle_keyboard_event(key);
}

core::container_component::container_component(const int x, const int y, const int width, const int height,
                                               const std::shared_ptr<console::console_view>& console_view)
    : component(x, y, width, height, console_view)
{
}
