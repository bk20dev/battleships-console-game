#include "list.hpp"

components::list_item::list_item(const int x, const int y, const std::shared_ptr<console::console>& console,
                                 const std::string& text, const std::function<void()>& on_select)
    : component(x, y, 0, 1, console), text(text), on_select(on_select)
{
}

void components::list_item::paint()
{
    const std::string styled_text = style.apply_to_text(text);
    console_view->write_at(0, 0, styled_text);
}

bool components::list_item::handle_keyboard_event(const console::keyboard::key& key)
{
    for (const auto& select_key : list_item_select_keys)
    {
        if (key.character == select_key)
        {
            if (on_select)
            {
                on_select();
            }
            return true;
        }
    }

    return component::handle_keyboard_event(key);
}

void components::list_item::focus()
{
    focusable::focus();
    set_style(selected_list_item_style);
}

void components::list_item::blur()
{
    focusable::blur();
    set_style(default_list_item_style);
}

void components::list_item::set_style(const console::style::style& new_style)
{
    styleable::set_style(new_style);
    invalidate();
}
