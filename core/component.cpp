#include "component.hpp"

void core::component::invalidate()
{
    needs_repaint = true;
}

void core::component::set_console_view(const std::shared_ptr<console::console>& console_view)
{
    const offset component_offset = {.x = position.x, .y = position.y};
    this->console_view = console_view->create_view(component_offset);
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

std::shared_ptr<console::console> create_console_view(
    const std::shared_ptr<console::console>& console, const core::offset& component_offset)
{
    if (!console)
    {
        return nullptr;
    }
    return console->create_view(component_offset);
}

core::component::component(const int x, const int y, const int width, const int height,
                           const std::shared_ptr<console::console>& console)
    : position{x, y}, size{width, height}, console_view(create_console_view(console, {x, y}))
{
}
