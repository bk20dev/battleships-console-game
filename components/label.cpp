#include "label.hpp"

components::label::label(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const std::string& text)
    : component(x, y, 0, 1, console), text(text)
{
}

void components::label::paint()
{
    const std::string styled_text = style.apply_to_text(text);
    console_view->write_at(0, 0, styled_text);

    component::paint();
}
