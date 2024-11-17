#include "console.hpp"

console::console::console(std::ostream& output_stream)
    : output_stream(output_stream)
{
}

void console::console::clear() const
{
    output_stream << "\033[2J" << std::flush;
}

void console::console::set_cursor(const bool display) const
{
    const char display_flag = display ? 'h' : 'l';
    output_stream << std::format("\x1b[?25{}", display_flag);
}

std::string console::console::move_to(const int x, const int y) const
{
    // Account for 1-based indexing in the terminal window
    const int row = y + 1, column = x + 1;
    return std::format("\033[{};{}H", row, column);
}

void console::console::write_at(const int x, const int y, const std::string& text)
{
    output_stream << move_to(x, y) << text;
}

std::shared_ptr<console::console_view> console::console::create_view(const core::position& offset) const
{
    return std::make_shared<console_view>(*this, offset);
}

std::string console::console_view::move_to(const int x, const int y) const
{
    const int row = y + position.y, column = x + position.x;
    return console::move_to(column, row);
}

console::console_view::console_view(const console& base_console, const core::position& position): console(base_console),
    position(position)
{
}

std::shared_ptr<console::console_view> console::console_view::create_view(const core::position& offset) const
{
    const core::position new_position = position + offset;
    return std::make_shared<console_view>(*this, new_position);
}
