#include "label.hpp"

#include "../utils/text.hpp"

void components::label::clear_previous_text_trail() const
{
    const int previous_painted_text_length = previous_painted_text.size();

    static constexpr console::style::style clear_style = {};
    const std::string empty_fill = utils::repeat_string(" ", previous_painted_text_length);

    const auto styled_empty_fill = clear_style.apply_to_text(empty_fill);

    console_view->write_at(0, 0, styled_empty_fill);
}

void components::label::paint_without_clearing_trail()
{
    const std::string styled_text = style.apply_to_text(text);
    console_view->write_at(0, 0, styled_text);

    previous_painted_text = text;

    component::paint();
}

components::label::label(const int x, const int y, const std::shared_ptr<console::console>& console,
                         const std::string& text)
    : component(x, y, text.size(), 1, console), text(text)
{
}

void components::label::set_text(const std::string& text)
{
    this->text = text;
    this->size.width = text.size();

    invalidate();
}

void components::label::paint()
{
    if (text != previous_painted_text)
    {
        clear_previous_text_trail();
    }

    paint_without_clearing_trail();
}
