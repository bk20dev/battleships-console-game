#pragma once
#include "../core/component.hpp"

namespace components
{
    enum input_type
    {
        TEXT,
        NUMBER,
    };

    class input final : public core::component
    {
        input_type input_type;

        std::string text{};

        int max_length;

        bool is_input_full() const
        {
            return text.size() >= max_length;
        }

        static bool is_key_valid(const console::keyboard::key& key)
        {
            return !key.is_special && isprint(key.character);
        }

    public:
        input(int x, int y, const std::shared_ptr<console::console>& console,
              components::input_type input_type, int max_length);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;
    };
}
