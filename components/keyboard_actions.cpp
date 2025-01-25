#include "keyboard_actions.hpp"

#include "../console/style.hpp"

namespace
{
    constexpr console::style::style key_to_press_style = {
    };

    constexpr console::style::style action_description_style = {
        .background_color = console::style::CYAN,
    };

    constexpr console::style::style background_style = {
        .background_color = console::style::CYAN,
    };

    constexpr console::style::style no_actions_available_hint_style = {
        .foreground_color = console::style::WHITE,
        .background_color = background_style.background_color,
    };
}

static int get_key_to_press_length(const std::string& key_to_press,
                                   const std::optional<int>& suggested_key_to_press_length)
{
    if (suggested_key_to_press_length.has_value())
    {
        return *suggested_key_to_press_length;
    }
    return key_to_press.length();
}

void components::keyboard_actions::footer_component::paint_background() const
{
    const models::rectangle background_rectangle = {
        .position = {0, 0},
        .size = {size.width, 1},
    };

    const std::string background_style_sequence = background_style.to_control_sequence();
    console_view->fill_rectangle(background_rectangle, " ", background_style_sequence);
}

void components::keyboard_actions::footer_component::paint_no_actions_available() const
{
    constexpr std::string hint_text = "No actions available";
    const std::string styled_hint = no_actions_available_hint_style.apply_to_text(hint_text);

    console_view->write_at(0, 0, styled_hint);
}

int components::keyboard_actions::footer_component::paint_keyboard_action(
    const int x, const keyboard_action& action) const
{
    static constexpr std::string PADDING = "  ";

    const auto [key_to_press, action_description, suggested_key_to_press_length] = action;
    const int key_to_press_length = get_key_to_press_length(key_to_press, suggested_key_to_press_length);

    const std::string styled_key_to_press = key_to_press_style.apply_to_text(key_to_press);
    const std::string styled_action_description = action_description_style.apply_to_text(action_description + PADDING);

    console_view->write_at(x, 0, styled_key_to_press);
    console_view->write_at(x + key_to_press_length, 0, styled_action_description);

    return key_to_press_length + action_description.length() + PADDING.length();
}

components::keyboard_actions::footer_component::footer_component(
    const int x, const int y, const int width, const std::shared_ptr<console::console>& console)
    : component(x, y, width, 1, console)
{
}

void components::keyboard_actions::footer_component::set_actions(const std::vector<keyboard_action>& keyboard_actions)
{
    this->keyboard_actions = keyboard_actions;
    invalidate();
}

void components::keyboard_actions::footer_component::paint()
{
    paint_background();

    if (keyboard_actions.empty())
    {
        paint_no_actions_available();
    }

    for (int current_x = 0; const auto& keyboard_action : keyboard_actions)
    {
        current_x += paint_keyboard_action(current_x, keyboard_action);
    }

    component::paint();
}
