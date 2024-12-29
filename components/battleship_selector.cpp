#include "battleship_selector.hpp"

#include "../utils/text.hpp"

components::battleship_selector_item::battleship_selector_item(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const battleship& battleship, const std::function<void()>& on_select)
    : list_item(x, y, console, on_select), normalized_battleship(battleship.normalized())
{
    const auto [width, height] = battleship.rectangle.size;

    size.width = std::max(width, height);
    size.height = std::min(width, height);
}

void components::battleship_selector_item::paint()
{
    const int battleship_length = normalized_battleship.rectangle.size.width;

    const std::string battleship_row = utils::repeat_string("\u2593", battleship_length * 2);
    const std::string styled_battleship_row = style.apply_to_text(battleship_row);

    console_view->write_at(0, 0, styled_battleship_row);

    list_item::paint();
}

void components::battleship_selector_item::focus()
{
    focusable::focus();
    set_style(selected_battleship_item_style);
}

void components::battleship_selector_item::blur()
{
    focusable::blur();
    set_style(default_battleship_item_style);
}
