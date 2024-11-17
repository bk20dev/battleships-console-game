#pragma once

#include <vector>

#include "dimension.hpp"
#include "../console/console.hpp"
#include "../console/keyboard.hpp"

namespace core
{
    class component : public std::enable_shared_from_this<component>
    {
    protected:
        bool needs_repaint = false;

        position position;
        size size;

        const std::shared_ptr<console::console> console;

        void invalidate();

    public:
        virtual ~component() = default;

        std::weak_ptr<component> parent{};

        [[nodiscard]] bool should_repaint() const;

        virtual void paint();

        virtual bool handle_keyboard_event(const console::keyboard::key& key);

        component(int x, int y, int width, int height,
                  const std::shared_ptr<console::console>& console);
    };

    class container_component final : public component
    {
    protected:
        std::vector<std::shared_ptr<component>> children_components{};

    public:
        void add_component(const std::shared_ptr<component>& other_component);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        container_component(int x, int y, int width, int height,
                            const std::shared_ptr<console::console>& console);
    };
}
