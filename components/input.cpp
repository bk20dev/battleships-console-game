#include "input.hpp"

#include <iostream>

bool components::input::is_input_full() const
{
    return text.size() >= max_length;
}

bool components::input::is_key_valid(const console::keyboard::key& key)
{
    return !key.is_special && isprint(key.character);
}

console::style::style components::input::get_input_style() const
{
    console::style::style result_style = {
        .text_effect = console::style::UNDERLINE,
    };

    // Merge user-applied styles with the default input style
    // and skip the FAINT effect, because it's used exclusively to indicate the disabled state.
    result_style.text_effect |= (style.text_effect & ~console::style::FAINT);

    if (!is_enabled)
    {
        result_style.text_effect |= console::style::FAINT;
    }

    if (is_focused)
    {
        result_style.foreground_color = style.foreground_color;
        result_style.background_color = style.background_color;
    }

    return result_style;
}

components::input::input(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const components::input_type input_type, const int max_length)
    : component(x, y, max_length, 1, console), input_type(input_type), max_length(max_length)
{
}

void components::input::paint()
{
    const console::style::style input_style = get_input_style();
    const std::string styled_text = input_style.apply_to_text(text);

    console_view->write_at(0, 0, styled_text);

    component::paint();
}

bool components::input::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key.character == console::keyboard::character::BACKSPACE)
    {
        if (!text.empty())
        {
            text.pop_back();
        }
        invalidate();
        return true;
    }

    if (!is_key_valid(key))
    {
        return false;
    }

    if (is_input_full())
    {
        // Prevent interpreting the input as an action, by consuming the event
        return true;
    }

    const char character = static_cast<char>(key.character);

    switch (input_type)
    {
    case TEXT:
        text += character;
        invalidate();
        return true;
    case NUMBER:
        if (isdigit(character))
        {
            text += character;
            invalidate();
        }
        return true;
    }

    return component::handle_keyboard_event(key);
}
