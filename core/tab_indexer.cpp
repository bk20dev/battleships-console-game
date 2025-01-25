#include "tab_indexer.hpp"

#include <iostream>

#include "component.hpp"

int core::tab_indexer::get_next_component_index(const int delta) const
{
    const int number_of_components = connected_components.size();

    if (number_of_components == 0)
    {
        return 0;
    }

    const int current_component_index = focused_component_index;
    const int next_component_index = current_component_index + delta;

    return (next_component_index + number_of_components) % number_of_components;
}

std::shared_ptr<core::component_traits::focusable> core::tab_indexer::get_component_at_index(const int index) const
{
    if (index < 0 || index >= connected_components.size())
    {
        return nullptr;
    }
    return connected_components[index];
}

void core::tab_indexer::repaint_component_if_needed(
    const std::shared_ptr<component_traits::focusable>& component_to_repaint)
{
    if (const auto component = std::dynamic_pointer_cast<class component>(component_to_repaint))
    {
        if (component->should_repaint())
        {
            component->paint();
        }
    }
}

void core::tab_indexer::blur(const std::shared_ptr<component_traits::focusable>& component_to_focus)
{
    if (const auto& focusable = component_to_focus)
    {
        focusable->blur();
    }
    repaint_component_if_needed(component_to_focus);
}

void core::tab_indexer::focus(const std::shared_ptr<component_traits::focusable>& component_to_blur)
{
    if (const auto& focusable = component_to_blur)
    {
        focusable->focus();
    }
    repaint_component_if_needed(component_to_blur);
}

int core::tab_indexer::connect_component(const std::shared_ptr<component_traits::focusable>& component_to_add)
{
    if (connected_components.empty())
    {
        focus(component_to_add);
    }
    connected_components.push_back(component_to_add);
    return connected_components.size() - 1;
}

int core::tab_indexer::focus_next_component(const int delta)
{
    const int component_to_focus_index = get_next_component_index(delta);

    const auto& currently_focused_component = get_component_at_index(focused_component_index);
    const auto& component_to_focus = get_component_at_index(component_to_focus_index);

    blur(currently_focused_component);
    focus(component_to_focus);

    focused_component_index = component_to_focus_index;
    return component_to_focus_index;
}
