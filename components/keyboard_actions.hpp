#pragma once
#include "../core/component.hpp"

namespace components::keyboard_actions
{
    static constexpr std::string VERTICAL_ARROWS = "\u2191/\u2193";
    static constexpr std::string ARROWS = "\u2190/\u2191/\u2192/\u2193";

    static constexpr int VERTICAL_ARROWS_LENGTH = 3;
    static constexpr int ARROWS_LENGTH = 7;

    struct keyboard_action
    {
        std::string key_to_press;
        std::string action_description;

        std::optional<int> key_to_press_length = std::nullopt;
    };

    class footer_component final : public core::component
    {
        std::vector<keyboard_action> keyboard_actions{};

        void paint_background() const;

        int paint_keyboard_action(int x, const keyboard_action& action) const;

    public:
        footer_component(int x, int y, int width, const std::shared_ptr<console::console>& console);

        void set_actions(const std::vector<keyboard_action>& keyboard_actions);

        void paint() override;
    };
}
