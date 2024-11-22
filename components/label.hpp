#pragma once

#include "../console/style.hpp"
#include "../core/component.hpp"

namespace components
{
    class label final : public core::component
    {
        std::string text;

        std::optional<console::style::style> style;

    public:
        label(int x, int y, const std::shared_ptr<console::console>& console, const std::string& text);

        void set_style(const std::optional<console::style::style>& style);

        void paint() override;
    };
}
