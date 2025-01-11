#pragma once

#include <vector>

#include "component_traits.hpp"

namespace core
{
    class tab_indexer
    {
        std::vector<std::shared_ptr<component_traits::focusable>> connected_components{};

        int focused_component_index = 0;

        [[nodiscard]] int get_next_component_index(int delta = 1) const;

        [[nodiscard]] std::shared_ptr<component_traits::focusable> get_component_at_index(int index) const;

        static void repaint_component_if_needed(
            const std::shared_ptr<component_traits::focusable>& component_to_repaint);

        static void blur(const std::shared_ptr<component_traits::focusable>& component_to_focus);

        static void focus(const std::shared_ptr<component_traits::focusable>& component_to_blur);

    public:
        int connect_component(const std::shared_ptr<component_traits::focusable>& component_to_add);

        int focus_next_component(int delta = 1);
    };
}
