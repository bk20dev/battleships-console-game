#include "console.hpp"

#include "../utils/text.hpp"

console::console::console(std::ostream& output_stream)
    : output_stream(output_stream)
{
}

void console::console::clear() const
{
    output_stream << "\x1b[0m" << std::flush; // Reset all styles
    system("clear");
}

void console::console::set_cursor_display(const bool display) const
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

void console::console::write_at(const int x, const int y, const std::string& text) const
{
    output_stream << move_to(x, y) << text;
}

void console::console::fill_rectangle(const core::rectangle& rectangle, const std::string& character,
                                      const std::string& line_prefix) const
{
    const auto [position, size] = rectangle;

    for (int y = position.y; y < position.y + size.height; y++)
    {
        const std::string row_text = utils::repeat_string(character, size.width);
        output_stream << move_to(position.x, y) << line_prefix << row_text;
    }
}

void console::console::rainbow_fill_row(const int x, const int y, const int width) const
{
    output_stream << move_to(x, y);
    for (int i = 0; i < width; i++)
    {
        const int color_code = (i % 8) + 100;
        output_stream << std::format("\033[{}m ", color_code);
    }
    output_stream << "\033[0m";
}

std::shared_ptr<console::console_view> console::console::create_view(const core::offset& offset) const
{
    const core::position view_position = {.x = offset.x, .y = offset.y};
    return std::make_shared<console_view>(*this, view_position);
}

void console::console::flush() const
{
    output_stream << std::flush;
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

std::shared_ptr<console::console_view> console::console_view::create_view(const core::offset& offset) const
{
    const core::position new_position = position + offset;
    return std::make_shared<console_view>(*this, new_position);
}
