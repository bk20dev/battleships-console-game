#pragma once

template <class C> requires std::is_base_of_v<core::component, C>
void core::container_component<C>::add_component(const std::shared_ptr<C>& other_component)
{
    children_components.emplace_back(other_component);
    other_component->parent = shared_from_this();
    other_component->set_console_view(console_view);
}

template <class C> requires std::is_base_of_v<core::component, C>
std::optional<std::shared_ptr<C>> core::container_component<C>::get_component_at_index(int index)
{
    if (index < 0 || index >= children_components.size())
    {
        return std::nullopt;
    }
    return children_components[index];
}

template <class C> requires std::is_base_of_v<core::component, C>
void core::container_component<C>::paint()
{
    for (const auto& child_component : children_components)
    {
        child_component->paint();
    }
    component::paint();
}

template <class C> requires std::is_base_of_v<core::component, C>
bool core::container_component<C>::handle_keyboard_event(const console::keyboard::key& key)
{
    for (const auto& child_component : children_components)
    {
        if (handle_keyboard_event_for_child(key, child_component))
        {
            return true;
        }
    }
    return component::handle_keyboard_event(key);
}

template <class C> requires std::is_base_of_v<core::component, C>
core::container_component<C>::container_component(const int x, const int y, const int width, const int height,
                                                  const std::shared_ptr<console::console>& console)
    : component(x, y, width, height, console)
{
}
