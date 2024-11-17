#include "console.hpp"

console::console::console(std::ostream& output_stream)
    : output_stream(output_stream)
{
}

void console::console::clear() const
{
    output_stream << "\033[2J" << std::flush;
}

void console::console::move_to(const int x, const int y) const
{
    // Account for 1-based indexing in the terminal window
    const int row = y + 1;
    const int column = x + 1;

    output_stream << std::format("\033[{};{}H", row, column);
}

std::shared_ptr<console::console_view> console::console::create_view(const core::position& offset) const
{
    return std::make_shared<console_view>(*this, offset);
}

console::console_view::console_view(const console& base_console, const core::position& position): console(base_console),
    position(position)
{
}

void console::console_view::move_to(const int x, const int y) const
{
    const int column = position.x + x;
    const int row = position.y + y;
    console::move_to(column, row);
}

std::shared_ptr<console::console_view> console::console_view::create_view(const core::position& offset) const
{
    const core::position new_position = position + offset;
    return std::make_shared<console_view>(*this, new_position);
}
