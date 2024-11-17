#include "label.hpp"

components::label::label(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const std::string& text)
    : component(x, y, 0, 1, console), text(text)
{
}

void components::label::paint()
{
    console_view->write_at(0, 0, text);
    component::paint();
}
