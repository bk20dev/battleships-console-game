#include "remaining_battleship_viewer.hpp"

#include "battleship.hpp"
#include "../constants/controls.hpp"
#include "../constants/style.hpp"

namespace
{
    constexpr core::size pixel_size = {.width = 2, .height = 1};
}

bool components::remaining_battleship_viewer::is_battleship_present(const models::battleship& battleship_to_find,
                                                                    const std::vector<models::battleship>& battleships)
{
    return std::ranges::any_of(battleships, [battleship_to_find](const models::battleship& current_battleship)
    {
        return battleship_to_find.id == current_battleship.id;
    });
}

static console::style::style get_battleship_style(const bool is_destroyed)
{
    using namespace constants::style::battleship;
    if (is_destroyed) return destroyed_style;
    return default_style;
}

void components::remaining_battleship_viewer::paint_battleship(const int y, models::battleship battleship_to_paint,
                                                               const bool is_destroyed) const
{
    battleship_to_paint.rectangle.position = {.x = 0, .y = y};

    const auto battleship_style = get_battleship_style(is_destroyed);
    battleship::paint(console_view, battleship_to_paint, battleship_style, pixel_size);
}

void components::remaining_battleship_viewer::paint_battleships() const
{
    int current_y = 0;

    for (const auto& battleship : available_battleships)
    {
        const auto normalized_battleship = battleship.normalized();
        const bool is_battleship_destroyed = is_battleship_present(battleship, destroyed_battleships);

        paint_battleship(current_y, normalized_battleship, is_battleship_destroyed);

        current_y += normalized_battleship.rectangle.size.height;
    }
}

components::remaining_battleship_viewer::remaining_battleship_viewer(
    const int x, const int y, const std::shared_ptr<console::console>& console,
    const std::vector<models::battleship>& available_battleships,
    const std::vector<models::battleship>& destroyed_battleships)
    : component(x, y, 0, 0, console),
      available_battleships(available_battleships),
      destroyed_battleships(destroyed_battleships)
{
}

void components::remaining_battleship_viewer::paint()
{
    paint_battleships();
    component::paint();
}
