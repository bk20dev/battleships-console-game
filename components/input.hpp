#pragma once

#include "../core/component.hpp"
#include "../core/component_traits.hpp"

namespace components
{
    enum input_type
    {
        TEXT,
        NUMBER,
    };

    class input final : public core::component,
                        public core::component_traits::focusable
    {
        input_type input_type;

        int max_length;

        std::string text{};
        std::string placeholder{};

        bool is_input_full() const;

        static bool is_key_valid(const console::keyboard::key& key);

        console::style::style get_input_style(bool is_placeholder = false) const;

    public:
        input(int x, int y, const std::shared_ptr<console::console>& console,
              components::input_type input_type, int max_length,
              const std::string& placeholder = "", const std::string& text = "");

        void set_text(const std::string& new_text);

        void set_placeholder(const std::string& new_placeholder);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void focus() override;

        void blur() override;
    };
}
