#pragma once

#include <array>
#include <string>

namespace console::style
{
    static constexpr std::string style_control_sequence = "m";

    enum color
    {
        DEFAULT = 39,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        BRIGHT_BLACK = 90,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_BLUE = 94,
        BRIGHT_MAGENTA = 95,
        BRIGHT_CYAN = 96,
        BRIGHT_WHITE = 97,
    };

    int get_foreground_color_code(const color& color);

    int get_background_color_code(const color& color);

    enum text_effect
    {
        BOLD = 1,
        FAINT = 2,
        ITALIC = 4,
        UNDERLINE = 8,
    };

    constexpr std::array text_effects = {BOLD, FAINT, ITALIC, UNDERLINE};

    int get_effect_code(const text_effect& effect);

    struct style
    {
        color foreground_color = DEFAULT;
        color background_color = DEFAULT;
        int text_effect = 0;

        [[nodiscard]] std::string to_control_sequence() const;

        [[nodiscard]] std::string apply_to_text(const std::string& text) const;
    };
}
