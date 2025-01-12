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

// ReSharper disable CppDFAConstantConditions
console::style::style components::input::get_input_style(const bool is_placeholder) const
{
    console::style::style result_style;

    if (is_enabled && is_focused)
    {
        result_style.background_color = console::style::CYAN;
        result_style.foreground_color = console::style::BRIGHT_WHITE;
    }
    else if (is_enabled && !is_focused)
    {
        result_style.background_color = console::style::WHITE;
        result_style.foreground_color = console::style::DEFAULT;
    }
    else if (!is_enabled && is_focused)
    {
        result_style.background_color = console::style::BRIGHT_BLACK;
        result_style.foreground_color = console::style::CYAN;
    }
    else if (!is_enabled && !is_focused)
    {
        result_style.background_color = console::style::BRIGHT_BLACK;
        result_style.foreground_color = console::style::DEFAULT;
    }

    if (is_placeholder)
    {
        result_style.text_effect |= console::style::FAINT;
    }

    return result_style;
}

components::input::input(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const components::input_type input_type, const int max_length,
                         const std::string& placeholder, const std::string& text)
    : component(x, y, max_length, 1, console),
      input_type(input_type),
      max_length(max_length),
      text(text),
      placeholder(placeholder)
{
}

void components::input::set_text(const std::string& new_text)
{
    text = new_text;
}

void components::input::set_placeholder(const std::string& new_placeholder)
{
    placeholder = new_placeholder;
}

std::string pad_right(const std::string& text, const size_t text_length, const int desired_length,
                      const char fill = ' ')
{
    std::string padded_text = text;

    const size_t spaces_to_add = desired_length - text_length;
    padded_text.append(spaces_to_add, fill);

    return padded_text;
}

void components::input::paint()
{
    const bool paint_placeholder = text.empty();

    const std::string text_to_print = paint_placeholder ? placeholder : text;
    const std::string padded_text = pad_right(text_to_print, text_to_print.size(), max_length, ' ');

    const console::style::style input_style = get_input_style(paint_placeholder);
    const std::string styled_text = input_style.apply_to_text(padded_text);

    console_view->write_at(0, 0, styled_text);

    component::paint();
}

bool components::input::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key == console::keyboard::character::BACKSPACE)
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

void components::input::focus()
{
    focusable::focus();
    invalidate();
}

void components::input::blur()
{
    focusable::blur();
    invalidate();
}
