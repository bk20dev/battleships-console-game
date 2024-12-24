#pragma once

template <class C> requires std::is_base_of_v<core::component, C>
void core::container_component<C>::add_component(const std::shared_ptr<C>& other_component)
{
    children_components.emplace_back(other_component);
    other_component->parent = shared_from_this();
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

template <class C> requires std::is_base_of_v<core::component, C>
core::container_component<C>::container_component(const int x, const int y, const int width, const int height,
                                                  const std::shared_ptr<console::console>& console)
    : component(x, y, width, height, console)
{
}
