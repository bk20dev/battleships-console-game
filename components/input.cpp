#include "input.hpp"

#include <iostream>

components::input::input(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const components::input_type input_type, const int max_length)
    : component(x, y, max_length, 1, console), input_type(input_type), max_length(max_length)
{
}

void components::input::paint()
{
    console_view->write_at(0, 0, text);
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
