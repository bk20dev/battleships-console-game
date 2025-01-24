#include "player.hpp"

#include "../utils/vector.tpp"

bool engine::player::is_opponent_bullet_present(const models::bullet& bullet_to_find) const
{
    return std::ranges::any_of(
        opponent_bullets,
        [bullet_to_find](const models::bullet& current_bullet)
        {
            return current_bullet == bullet_to_find;
        });
}

std::optional<models::battleship> engine::player::get_battleship(const core::position& position) const
{
    return utils::find_if_or_null(
        placed_battleships,
        [position](const models::battleship& current_battleship)
        {
            return current_battleship.rectangle.intersects(position);
        });
}

bool engine::player::is_battleship_destroyed(const models::battleship& battleship_to_check) const
{
    const long shots_on_target = std::ranges::count_if(
        opponent_bullets,
        [battleship_to_check](const models::bullet& current_bullet)
        {
            return battleship_to_check.rectangle.intersects(current_bullet.position);
        });
    const int battleship_size = battleship_to_check.rectangle.get_area();
    return shots_on_target >= battleship_size;
}

bool engine::player::get_is_board_ready() const
{
    return is_board_ready;
}

bool engine::player::get_is_currently_playing() const
{
    return currently_plays;
}

const std::vector<models::battleship>& engine::player::get_placed_battleships() const
{
    return placed_battleships;
}

const std::vector<models::bullet>& engine::player::get_opponent_bullets() const
{
    return opponent_bullets;
}

const std::vector<core::position>& engine::player::get_damaged_battleship_parts() const
{
    return damaged_battleship_parts;
}

const std::vector<models::battleship>& engine::player::get_destroyed_battleships() const
{
    return destroyed_battleships;
}

void engine::player::set_placed_battleships(const std::vector<models::battleship>& placed_battleships)
{
    this->placed_battleships = placed_battleships;
}

bool engine::player::shoot_with(const models::bullet& bullet)
{
    if (is_opponent_bullet_present(bullet))
    {
        return false;
    }
    opponent_bullets.push_back(bullet);

    const auto hit_battleship = get_battleship(bullet.position);

    if (!hit_battleship)
    {
        return true;
    }

    damaged_battleship_parts.push_back(bullet.position);
    if (on_add_damaged_battleship_part)
    {
        on_add_damaged_battleship_part(bullet.position);
    }

    if (is_battleship_destroyed(*hit_battleship))
    {
        destroyed_battleships.push_back(*hit_battleship);
        if (on_destroyed_battleship)
        {
            on_destroyed_battleship(*hit_battleship);
        }
    }
    return true;
}

engine::player::player(
    const std::function<void(const core::position& damaged_battleship_part)>& on_add_damaged_battleship_part,
    const std::function<void(const models::battleship& destroyed_battleship)>& on_destroyed_battleship)
    : on_add_damaged_battleship_part(on_add_damaged_battleship_part),
      on_destroyed_battleship(on_destroyed_battleship)
{
}

void engine::player::add_damaged_battleship_part(const core::position& damaged_battleship_part)
{
    damaged_battleship_parts.push_back(damaged_battleship_part);
}

void engine::player::add_destroyed_battleship(const models::battleship& destroyed_battleship)
{
    destroyed_battleships.push_back(destroyed_battleship);
}

void engine::player::set_board_ready()
{
    is_board_ready = true;
}

void engine::player::set_currently_plays(const bool currently_plays)
{
    this->currently_plays = currently_plays;
}
