#include "board_generator.hpp"

#include <random>

static bool check_board_fully_contains(const models::size& board_size, const models::battleship& battleship_to_check)
{
    const auto [position, size] = battleship_to_check.rectangle;

    if (position.x < 0 || position.y < 0) return false;
    if (position.x + size.width > board_size.width) return false;
    if (position.y + size.height > board_size.height) return false;
    return true;
}

/**
 * Checks whether a battleship fits on a game board and does not intersect with other battleships.
 */
[[nodiscard]] static bool check_battleship_placement(
    const models::battleship& battleship_to_check,
    const std::vector<models::position>& positions,
    // ReSharper disable once CppDFAConstantParameter
    const int occupied_positions, const models::size& board_size)
{
    if (!check_board_fully_contains(board_size, battleship_to_check))
    {
        return false;
    }

    for (int i = 0; i < occupied_positions; i++)
    {
        const models::position current_position = positions[i];
        if (battleship_to_check.rectangle.intersects(current_position))
        {
            return false;
        }
    }

    return true;
}

/**
 * Moves all positions that intersect with the battleship's forbidden area to the end of occupied positions.
 */
static int place_battleship(
    // ReSharper disable once CppDFAConstantParameter
    std::vector<models::position>& positions, int occupied_positions,
    const models::battleship& battleship_to_check)
{
    const models::rectangle forbidden_area = battleship_to_check.rectangle.expanded_by(1);

    for (int i = occupied_positions; i < positions.size(); i++)
    {
        if (forbidden_area.intersects(positions[i]))
        {
            std::swap(positions[occupied_positions], positions[i]);
            occupied_positions++;
        }
    }

    return occupied_positions;
}

static bool generate_board(
    std::vector<models::position>& positions, int occupied_positions,
    std::vector<models::battleship>& battleships, int placed_battleships,
    const models::size& board_size);

/**
 * Tries to simulate the placement of other battleships, based on the given battleship setting.
 */
static bool simulate_battleship_placement( // NOLINT(*-no-recursion)
    // ReSharper disable once CppDFAConstantParameter
    std::vector<models::position>& positions, const int occupied_positions,
    const models::battleship& current_battleship, std::vector<models::battleship>& battleships,
    // ReSharper disable once CppDFAConstantParameter
    const int placed_battleships, const models::size& board_size
)
{
    if (!check_battleship_placement(current_battleship, positions, occupied_positions, board_size))
    {
        return false;
    }

    const int new_occupied_positions = place_battleship(positions, occupied_positions, current_battleship);
    if (generate_board(positions, new_occupied_positions, battleships, placed_battleships + 1, board_size))
    {
        return true;
    }

    return false;
}

/**
 * Tries to simulate the placement of other battleships, based on the given battleship setting.
 * The method tries two possible rotations at random.
 */
static bool simulate_vertical_horizontal_battleship_placement( // NOLINT(*-no-recursion)
    // ReSharper disable once CppDFAConstantParameter
    std::vector<models::position>& positions, const int occupied_positions,
    // ReSharper disable once CppDFAConstantParameter
    models::battleship& current_battleship, std::vector<models::battleship>& battleships, const int placed_battleships,
    const models::size& board_size, std::mt19937& random_number_generator)
{
    // Determine if the battleship should be rotated at the beginning or not.
    auto direction_magic_factor = std::uniform_int_distribution(0, 1);
    if (direction_magic_factor(random_number_generator) == 1)
    {
        current_battleship.rectangle = current_battleship.rectangle.rotated();
    }

    if (simulate_battleship_placement(
        positions, occupied_positions, current_battleship, battleships, placed_battleships, board_size))
    {
        return true;
    }

    // Also, try the other rotation if the first one failed.
    current_battleship.rectangle = current_battleship.rectangle.rotated();

    if (simulate_battleship_placement(
        positions, occupied_positions, current_battleship, battleships, placed_battleships, board_size))
    {
        return true;
    }

    return false;
}

static bool generate_board( // NOLINT(*-no-recursion)
    // ReSharper disable once CppDFAConstantParameter
    std::vector<models::position>& positions, const int occupied_positions,
    // ReSharper disable once CppDFAConstantParameter
    std::vector<models::battleship>& battleships, const int placed_battleships,
    const models::size& board_size)
{
    if (placed_battleships >= battleships.size())
    {
        return true;
    }

    static std::random_device random_device;
    static std::mt19937 random_number_generator(random_device());

    const auto first_available_position = positions.begin() + occupied_positions;
    std::shuffle(first_available_position, positions.end(), random_number_generator);

    const int current_battleship_index = placed_battleships;

    for (auto current_position = first_available_position; current_position != positions.end(); ++current_position)
    {
        models::battleship& current_battleship = battleships[current_battleship_index];
        current_battleship.rectangle.position = *current_position;

        if (simulate_vertical_horizontal_battleship_placement(
            positions, occupied_positions,
            current_battleship, battleships, placed_battleships,
            board_size, random_number_generator))
        {
            return true;
        }
    }

    return false;
}

/**
 * Generates a vector of all coordinates in a plane segment.
 */
static std::vector<models::position> generate_board_positions(const models::size& board_size)
{
    const int cell_count = board_size.width * board_size.height;
    std::vector<models::position> generated_positions(cell_count);

    std::ranges::generate(generated_positions, [n = 0, board_size]() mutable
    {
        const int x = n % board_size.width;
        const int y = n / board_size.width;

        n += 1;

        return models::position{.x = x, .y = y};
    });

    return generated_positions;
}

/**
 * Generates a new game board with randomly placed battleships.
 */
std::vector<models::battleship> engine::generator::random_board(
    std::vector<models::battleship> available_battleships,
    const models::size& board_size)
{
    std::vector<models::position> positions = generate_board_positions(board_size);
    generate_board(positions, 0, available_battleships, 0, board_size);

    return available_battleships;
}
