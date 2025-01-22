#pragma once

#include "../core/component.hpp"
#include "../core/component_traits.hpp"

namespace components
{
    class button : public core::component, public core::component_traits::focusable,
                   public core::component_traits::styleable
    {
    protected:
        std::function<void()> on_select;

    public:
        button(int x, int y, int width, int height, const std::shared_ptr<console::console>& console,
               const std::function<void()>& on_select);

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void focus() override;

        void blur() override;

        void set_style(const console::style::style& new_style) override;
    };

    class text_button final : public button
    {
        std::string text;
        std::string previous_painted_text;

        void clear_previous_text_trail() const;

    public:
        text_button(int x, int y, const std::shared_ptr<console::console>& console,
                    const std::string& text = "", const std::function<void()>& on_select = nullptr);

        void set_text(const std::string&);

        void paint() override;
    };
}
