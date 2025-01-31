#pragma once

#include <vector>

#include "../console/console.hpp"
#include "../console/keyboard.hpp"
#include "../models/position.hpp"

namespace core
{
    class component : public std::enable_shared_from_this<component>
    {
    protected:
        bool needs_repaint = false;

        models::position position;
        models::size size;

        std::shared_ptr<console::console> console_view;        /**
         * Marks the component to be repainted.
         */
        void invalidate();

        /**
         * Dispatches a keyboard event for a given component.
         * @param key The pressed key.
         * @param child_component Component this event will be dispatched to.
         * @return True if the component handled the event.
         */
        static bool handle_keyboard_event_for_child(const console::keyboard::key& key,
                                                    std::shared_ptr<component> child_component);

    public:
        virtual ~component() = default;

        std::weak_ptr<component> parent{};

        /**
         * Creates a subview of the provided console view at this component's offset and sets it as a console_view.
         * @param console_view Console view to create a subview of.
         */
        void set_console_view(const std::shared_ptr<console::console>& console_view);

        /**
         * Checks whether the component should be repainted.
         * @return True if the component needs repaint.
         */
        [[nodiscard]] bool should_repaint() const;

        /**
         * Paints the component on the screen.
         */
        virtual void paint();

        /**
         * Attempts to handle the keyboard event.
         * @param key Pressed key.
         * @return True if the event was handled.
         */
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
        /**
         * Adds a child component and sets the container component as a parent.
         * @param other_component Component to add.
         */
        virtual void add_component(const std::shared_ptr<C>& other_component);

        /**
         * Returns a child component at a given index.
         * @param index Index of the child component.
         * @return Child component if found, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<std::shared_ptr<C>> get_component_at_index(int index);

        void paint() override;

        /**
         * Dispatches a keyboard event to all children and returns as soon as one of them handled the event.
         * @param key Pressed key.
         * @return True if the keyboard event was handled by any child component.
         */
        bool handle_keyboard_event(const console::keyboard::key& key) override;

        container_component(int x, int y, int width, int height,
                            const std::shared_ptr<console::console>& console);
    };
}

#include "component.tpp"
