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

void components::battleship_selector::add_battleships_to_selector_list(
    const std::vector<battleship>& battleships_to_add)
{
    for (int i = 0; i < battleships_to_add.size(); i++)
    {
        const battleship battleship_to_add = battleships_to_add[i];
        const auto battleship_selector_item = std::make_shared<components::battleship_selector_item>(
            0, i, console_view, battleship_to_add, [this, battleship_to_add]
            {
                select_battleship(battleship_to_add);
            });
        battleship_selector_list->add_component(battleship_selector_item);
    }
}

void components::battleship_selector::select_battleship(const battleship& battleship_to_select) const
{
    on_battleship_select(battleship_to_select);
}

components::battleship_selector::battleship_selector(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<battleship>& battleships,
    const std::shared_ptr<list<battleship_selector_item>>& battleship_selector_list,
    const std::function<void(const battleship& battleship)>& on_battleship_select)
    : component(x, y, 0, 0, console),
      battleships(battleships),
      battleship_selector_list(battleship_selector_list),
      on_battleship_select(on_battleship_select)
{
    add_battleships_to_selector_list(battleships);
}

components::battleship_selector::battleship_selector(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<battleship>& battleships,
    const std::function<void(const battleship& battleship)>& on_battleship_select)
    : battleship_selector(
        x, y, console, battleships,
        std::make_shared<list<battleship_selector_item>>(0, 0, console),
        on_battleship_select)
{
}

void components::battleship_selector::paint()
{
    battleship_selector_list->paint();
    component::paint();
}

bool components::battleship_selector::handle_keyboard_event(const console::keyboard::key& key)
{
    return battleship_selector_list->handle_keyboard_event(key);
}
