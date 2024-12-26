#include <iostream>

#include "components/list.hpp"
#include "core/component.hpp"

[[noreturn]] void keyboard_input_listener(const std::shared_ptr<console::keyboard::keyboard>& keyboard,
                                          const std::shared_ptr<core::component>& component)
{
    while (true)
    {
        const auto pressed_key = keyboard->get_pressed_key();
        component->handle_keyboard_event(pressed_key);
        std::cout << std::flush;
    }
}

class helper final : public core::component, public core::component_traits::focusable
{
public:
    helper(int x, int y, int width, int height, const std::shared_ptr<console::console>& console)
        : component(x, y, width, height, console)
    {
    }

    void focus() override
    {
        focusable::focus();
        invalidate();
    }

    void blur() override
    {
        focusable::blur();
        invalidate();
    }

    void paint() override
    {
        const std::string text_to_write = is_focused ? "SOME TEXT" : "some text";
        console_view->write_at(0, 0, text_to_write);
    }
};

int main()
{
    const auto keyboard = std::make_shared<console::keyboard::keyboard>();
    keyboard->listen_for_input();

    const auto console = std::make_shared<console::console>(std::cout);
    console->set_cursor_display(false);
    console->clear();

    const auto list = std::make_shared<components::list<helper>>(0, 0, console);

    list->add_component(std::make_shared<helper>(0, 0, 0, 0, console));
    list->add_component(std::make_shared<helper>(0, 1, 0, 0, console));
    list->add_component(std::make_shared<helper>(0, 2, 0, 0, console));
    list->add_component(std::make_shared<helper>(0, 3, 0, 0, console));

    list->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &list]()
    {
        keyboard_input_listener(keyboard, list);
    });

    keyboard_input_thread.join();

    return 0;
}
