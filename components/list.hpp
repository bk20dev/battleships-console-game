#pragma once

#include "../core/component.hpp"
#include "../core/component_traits.hpp"

template <class T, class... Bases>
concept is_base_of_multiple_v = (std::is_base_of_v<Bases, T> && ...);

namespace components
{
    template <class C> requires is_base_of_multiple_v<C, core::component, core::component_traits::focusable>
    class list final : public core::container_component<C>
    {
    protected:
        int selected_item_index = 0;

        [[nodiscard]] int get_new_selected_item_index(int index_delta) const;

        bool update_selected_item_index(const console::keyboard::key& key);

        static void update_item_style(const std::shared_ptr<C>& item, bool is_focused);

    public:
        void add_component(const std::shared_ptr<C>& other_component) override;

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        list(int x, int y, const std::shared_ptr<console::console>& console);
    };

    constexpr std::array list_item_select_keys = {
        console::keyboard::character::ENTER,
        console::keyboard::character::SPACE,
    };

    constexpr console::style::style default_list_item_style = {
    };

    constexpr console::style::style selected_list_item_style = {
        .background_color = console::style::color::CYAN
    };

    class list_item : public core::component, public core::component_traits::focusable,
                      protected core::component_traits::styleable
    {
    protected:
        std::function<void()> on_select;

    public:
        list_item(int x, int y, const std::shared_ptr<console::console>& console,
                  const std::function<void()>& on_select = nullptr);

        bool handle_keyboard_event(const console::keyboard::key& key) override;

        void focus() override;

        void blur() override;

        void set_style(const console::style::style& new_style) override;
    };

    class text_list_item : public list_item
    {
        std::string text;

    public:
        text_list_item(int x, int y, const std::shared_ptr<console::console>& console,
                       const std::string& text, const std::function<void()>& on_select);

        void paint() override;
    };
}

#include "list.tpp"
