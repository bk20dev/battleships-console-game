#pragma once

#include "../core/component.hpp"
#include "../core/component_traits.hpp"

namespace components
{
    class label final : public core::component, public core::component_traits::styleable
    {
        std::string text;
        std::string previous_painted_text;

        void clear_previous_text_trail() const;

    public:
        label(int x, int y, const std::shared_ptr<console::console>& console, const std::string& text = "");

        void set_text(const std::string& text);

        void paint() override;
    };
}
