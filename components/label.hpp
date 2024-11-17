#pragma once

#include "../core/component.hpp"

namespace components
{
    class label final : public core::component
    {
        std::string text;

    public:
        label(int x, int y, const std::shared_ptr<console::console>& console, const std::string& text);

        void paint() override;
    };
}
