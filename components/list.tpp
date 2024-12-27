#pragma once

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
int components::list<C>::get_new_selected_item_index(const int index_delta) const
{
    const int children_component_count = this->children_components.size();

    if (children_component_count < 1)
    {
        return 0;
    }

    const int new_selected_item_index = selected_item_index + index_delta;
    return (new_selected_item_index + children_component_count) % children_component_count;
}

inline int get_item_index_delta(const int key_character)
{
    switch (key_character)
    {
    case console::keyboard::character::ARROW_UP:
        return -1;
    case console::keyboard::character::ARROW_DOWN:
        return 1;
    default:
        return 0;
    }
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
bool components::list<C>::update_selected_item_index(const console::keyboard::key& key)
{
    if (!key.is_vertical_arrow())
    {
        return false;
    }

    const core::offset key_arrow_offset = key.get_arrow_offset();
    const int item_index_delta = key_arrow_offset.y;
    const int new_selected_item_index = get_new_selected_item_index(item_index_delta);

    if (new_selected_item_index == selected_item_index)
    {
        return false;
    }

    selected_item_index = new_selected_item_index;
    return true;
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
void components::list<C>::update_item_style(const std::shared_ptr<C>& item, const bool is_focused)
{
    if (is_focused)
    {
        item->focus();
    }
    else
    {
        item->blur();
    }

    if (item->should_repaint())
    {
        item->paint();
    }
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
void components::list<C>::add_component(const std::shared_ptr<C>& other_component)
{
    core::container_component<C>::add_component(other_component);

    const int children_component_count = this->children_components.size();
    if (children_component_count == 1)
    {
        update_item_style(other_component, /* is_focused */ true);
    }
    else
    {
        update_item_style(other_component, /* is_focused */ false);
    }
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
bool components::list<C>::handle_keyboard_event(const console::keyboard::key& key)
{
    const std::optional<std::shared_ptr<C>> selected_item = this->get_component_at_index(selected_item_index);

    if (selected_item.has_value())
    {
        const std::shared_ptr<C> selected_item_value = selected_item.value();
        if (this->handle_keyboard_event_for_child(key, selected_item_value))
        {
            return true;
        }
    }

    if (!update_selected_item_index(key))
    {
        return core::container_component<C>::handle_keyboard_event(key);
    }

    const std::optional<std::shared_ptr<C>> new_selected_item = this->get_component_at_index(selected_item_index);

    if (selected_item.has_value())
    {
        const std::shared_ptr<C> selected_item_value = selected_item.value();
        update_item_style(selected_item_value, /* is_focused */ false);
    }

    if (new_selected_item.has_value())
    {
        const std::shared_ptr<C> new_selected_item_value = new_selected_item.value();
        update_item_style(new_selected_item_value, /* is_focused */ true);
    }

    return true;
}

template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
components::list<C>::list(const int x, const int y, const std::shared_ptr<console::console>& console)
    : core::container_component<C>(x, y, 0, 0, console)
{
}
