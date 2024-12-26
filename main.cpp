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

int main()
{
    const auto keyboard = std::make_shared<console::keyboard::keyboard>();
    keyboard->listen_for_input();

    const auto console = std::make_shared<console::console>(std::cout);
    console->set_cursor_display(false);
    console->clear();

    const auto list = std::make_shared<components::list<components::list_item>>(0, 0, console);

    list->add_component(std::make_shared<components::list_item>(0, 0, console, "first"));
    list->add_component(std::make_shared<components::list_item>(0, 1, console, "second"));
    list->add_component(std::make_shared<components::list_item>(0, 2, console, "third"));

    list->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &list]()
    {
        keyboard_input_listener(keyboard, list);
    });

    keyboard_input_thread.join();

    return 0;
}
