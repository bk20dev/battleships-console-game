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

        std::shared_ptr<console::console> console_view;

        void invalidate();

        static bool handle_keyboard_event_for_child(const console::keyboard::key& key,
                                                    const std::shared_ptr<component>& child_component);

    public:
        virtual ~component() = default;

        std::weak_ptr<component> parent{};

        void set_console_view(const std::shared_ptr<console::console>& console_view);

        [[nodiscard]] bool should_repaint() const;

        virtual void paint();

        virtual bool handle_keyboard_event(const console::keyboard::key& key);

        component(int x, int y, int width, int height,
                  const std::shared_ptr<console::console>& console);
    };

    template <class C> requires std::is_base_of_v<component, C>
    class container_component : public component
    {
    protected:
        std::vector<std::shared_ptr<C>> children_components{};

    public:
        virtual void add_component(const std::shared_ptr<C>& other_component);

        [[nodiscard]] std::optional<std::shared_ptr<C>> get_component_at_index(int index);

        void paint() override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        container_component(int x, int y, int width, int height,
                            const std::shared_ptr<console::console>& console);
    };
}

#include "component.tpp"
