#include "battleship_selector.hpp"

#include "battleship.hpp"
#include "../constants/style.hpp"
#include "../utils/vector.tpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};
}

static bool is_battleship_contained(const models::battleship& battleship_to_check,
                                    const std::vector<models::battleship>& battleships_vector)
{
    return utils::contains(
        battleships_vector,
        [battleship_to_check](const models::battleship& current_battleship)
        {
            return current_battleship.id == battleship_to_check.id;
        });
}

bool components::battleship_selector::is_battleship_placed(const models::battleship& battleship_to_check) const
{
    return is_battleship_contained(battleship_to_check, placed_battleships);
}

bool components::battleship_selector::is_battleship_misplaced(const models::battleship& battleship_to_check) const
{
    return is_battleship_contained(battleship_to_check, misplaced_battleships);
}

static console::style::style get_battleship_style(const bool is_selected, const bool is_placed, const bool is_misplaced)
{
    using namespace constants::style::battleship;
    if (is_selected) return selected_style;
    if (is_misplaced) return misplaced_style;
    if (is_placed) return placed_style;
    return default_style;
}

void components::battleship_selector::paint_battleships() const
{
    int current_battleship_y = 0;

    for (int battleship_index = 0; battleship_index < all_battleships.size(); battleship_index++)
    {
        const models::battleship& battleship = all_battleships[battleship_index];

        models::battleship battleship_to_paint = battleship.normalized();
        battleship_to_paint.rectangle.position = {.x = 0, .y = current_battleship_y};

        const bool is_selected = battleship_index == selected_battleship_index;
        const bool is_placed = is_battleship_placed(battleship_to_paint);
        const bool is_misplaced = is_battleship_misplaced(battleship_to_paint);

        console::style::style battleship_style = get_battleship_style(is_selected, is_placed, is_misplaced);
        battleship::paint(console_view, battleship_to_paint, battleship_style, pixel_size);

        current_battleship_y += battleship_to_paint.rectangle.size.height;
    }
}

void components::battleship_selector::update_selected_battleship_index(const int selected_index_delta)
{
    const int battleship_count = all_battleships.size();

    if (battleship_count == 0)
    {
        selected_battleship_index = 0;
        return;
    }

    const auto clamped_battleship_index = std::clamp(selected_battleship_index, 0, battleship_count - 1);

    // In case the selected_battleship_index is out of bounds, simply treat the update as a corrective action
    // without taking the selected_index_delta into account.
    // This can happen when some items were removed from the all_battleships vector.
    if (clamped_battleship_index != selected_battleship_index)
    {
        selected_battleship_index = clamped_battleship_index;
        return;
    }

    const auto updated_battleship_index = clamped_battleship_index + selected_index_delta;

    // Loop the selection, by adding and dividing by the battleship count.
    selected_battleship_index = (updated_battleship_index + battleship_count) % battleship_count;
}

std::optional<models::battleship> components::battleship_selector::get_selected_battleship() const
{
    if (selected_battleship_index < 0 || selected_battleship_index >= all_battleships.size())
    {
        return std::nullopt;
    }
    return all_battleships[selected_battleship_index];
}

components::battleship_selector::battleship_selector(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<models::battleship>& all_battleships,
    const std::vector<models::battleship>& placed_battleships,
    const std::vector<models::battleship>& misplaced_battleships,
    const std::function<void(const models::battleship& battleship_to_select)>& on_battleship_select)
    : component(x, y, 0, 0, console), all_battleships(all_battleships), placed_battleships(placed_battleships),
      misplaced_battleships(misplaced_battleships), on_battleship_select(on_battleship_select)
{
}

void components::battleship_selector::paint()
{
    paint_battleships();
    component::paint();
}

bool components::battleship_selector::handle_keyboard_event(const console::keyboard::key& key)
{
    if (key == console::keyboard::character::ENTER || key == console::keyboard::character::SPACE)
    {
        if (const std::optional<models::battleship> selected_battleship = get_selected_battleship())
        {
            on_battleship_select(*selected_battleship);
        }
        return true;
    }

    if (!key.is_vertical_arrow())
    {
        return component::handle_keyboard_event(key);
    }

    const core::offset key_arrow_offset = key.get_arrow_offset();
    update_selected_battleship_index(key_arrow_offset.y);

    invalidate();
    return true;
}
