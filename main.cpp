#include <iostream>

#include "components/input.hpp"
#include "components/label.hpp"
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

    const auto form = std::make_shared<core::container_component>(0, 0, 100, 100, console);
    const auto name_input = std::make_shared<components::input>(0, 0, console, components::input_type::TEXT, 16);

    name_input->focus();
    name_input->disable();

    form->add_component(name_input);

    form->paint();
    std::cout << std::flush;

    std::thread keyboard_input_thread([&keyboard, &form]()
    {
        keyboard_input_listener(keyboard, form);
    });

    keyboard_input_thread.join();

    return 0;
}
