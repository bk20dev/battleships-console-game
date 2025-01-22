#include "button.hpp"

#include "../constants/controls.hpp"
#include "../constants/style.hpp"
#include "../utils/text.hpp"

components::button::button(
    const int x, const int y, const int width, const int height, const std::shared_ptr<console::console>& console,
    const std::function<void()>& on_select)
    : component(x, y, width, height, console), on_select(on_select)
{
    button::blur();
}

bool components::button::handle_keyboard_event(const console::keyboard::key& key)
{
    for (const auto& select_key : constants::controls::select_keys)
    {
        if (key == select_key)
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

void components::button::focus()
{
    focusable::focus();
    set_style(constants::style::general::selected_style);
}

void components::button::blur()
{
    focusable::blur();
    set_style(constants::style::general::deselected_style);
}

void components::button::set_style(const console::style::style& new_style)
{
    styleable::set_style(new_style);
    invalidate();
}

void components::text_button::clear_previous_text_trail() const
{
    const int previous_painted_text_length = previous_painted_text.size();

    static constexpr console::style::style clear_style = {};
    const std::string empty_fill = utils::repeat_string(" ", previous_painted_text_length);

    const auto styled_empty_fill = clear_style.apply_to_text(empty_fill);

    console_view->write_at(0, 0, styled_empty_fill);
}

components::text_button::text_button(const int x, const int y, const std::shared_ptr<console::console>& console,
                                     const std::string& text, const std::function<void()>& on_select)
    : button(x, y, text.length(), 1, console, on_select), text(text)
{
}

void components::text_button::set_text(const std::string& updated_text)
{
    text = updated_text;
    size.width = updated_text.size();

    invalidate();
}

void components::text_button::paint()
{
    if (text != previous_painted_text)
    {
        clear_previous_text_trail();
    }

    const std::string styled_text = style.apply_to_text(text);
    console_view->write_at(0, 0, styled_text);

    previous_painted_text = text;

    button::paint();
}
