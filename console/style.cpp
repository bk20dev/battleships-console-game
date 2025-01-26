#include "style.hpp"

#include <format>
#include <string>

/**
 * Returns a Control Sequence Introducer. Read more at https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
 * @param attribute The n part of CSI, e.g. 31.
 * @param code The code part of CSI, e.g. m (SGR).
 * @return The attribute preceded by ANSI escape sequence.
 */
std::string create_control_sequence(const std::string& attribute, const std::string& code)
{
    return std::format("\x1b[{}{}", attribute, code);
}

int console::style::get_foreground_color_code(const color& color)
{
    return color;
}

int console::style::get_background_color_code(const color& color)
{
    return get_foreground_color_code(color) + 10;
}

int console::style::get_effect_code(const text_effect& effect)
{
    switch (effect)
    {
    case BOLD:
        return 1;
    case FAINT:
        return 2;
    case ITALIC:
        return 3;
    case UNDERLINE:
        return 4;
    }

    const int effect_code = effect;
    throw std::runtime_error(std::format("Unknown text effect. (effect={})", effect_code));
}

std::string console::style::style::to_control_sequence() const
{
    std::string style_sequence = "0";

    // Foreground color
    const int foreground_color_code = get_foreground_color_code(foreground_color);
    style_sequence += std::format(";{}", foreground_color_code);

    // Background color
    const int background_color_code = get_background_color_code(background_color);
    style_sequence += std::format(";{}", background_color_code);

    // Text effects
    for (const auto& effect : text_effects)
    {
        if (text_effect & effect)
        {
            const int effect_code = get_effect_code(effect);
            style_sequence += std::format(";{}", effect_code);
        }
    }

    return create_control_sequence(style_sequence, style_control_sequence);
}

std::string console::style::style::apply_to_text(const std::string& text) const
{
    const std::string color_sequence = to_control_sequence();
    return color_sequence + text;
}
