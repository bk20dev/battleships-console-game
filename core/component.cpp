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
                           const std::shared_ptr<console::console>& console)
    : position{x, y}, size{width, height}, console_view(console->create_view({x, y}))
{
}
