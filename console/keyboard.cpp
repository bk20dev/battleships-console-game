#include "keyboard.hpp"

#include <unistd.h>
#include <sys/termios.h>
#include <sys/time.h>

int read_character_from_stdin()
{
    char key = 0;
    if (read(STDIN_FILENO, &key, sizeof(key)) != 1)
    {
        const std::string error_message = "Failed to read character from stdin.";
        throw std::runtime_error(error_message);
    }
    return key;
}

/**
 * Checks whether any key is currently pressed.
 * @return true if any key is currently pressed, false otherwise
 */
bool is_key_pressed()
{
    timeval timeout{0L, 0L};

    fd_set file_descriptors;
    FD_ZERO(&file_descriptors);
    FD_SET(STDIN_FILENO, &file_descriptors);

    return select(STDIN_FILENO + 1, &file_descriptors, nullptr, nullptr, &timeout);
}

console::keyboard::key get_pressed_key_from_stdin()
{
    const int pressed_key = read_character_from_stdin();
    if (pressed_key != console::keyboard::ESCAPE || !is_key_pressed())
    {
        return console::keyboard::key{.is_special = false, .character = pressed_key};
    }

    read_character_from_stdin(); // Escape sequences contain `[` after the escape character.
    const int pressed_escape_key = read_character_from_stdin();

    return console::keyboard::key{.is_special = true, .character = pressed_escape_key};
}

bool console::keyboard::key::operator==(const enum character& character) const
{
    if (is_special) return false;
    return std::toupper(this->character) == character;
}

bool console::keyboard::key::is_vertical_arrow() const
{
    if (!is_special) return false;
    return character == ARROW_UP || character == ARROW_DOWN;
}

bool console::keyboard::key::is_horizontal_arrow() const
{
    if (!is_special) return false;
    return character == ARROW_RIGHT || character == ARROW_LEFT;
}

bool console::keyboard::key::is_arrow() const
{
    return is_vertical_arrow() || is_horizontal_arrow();
}

models::offset console::keyboard::key::get_arrow_offset() const
{
    if (!is_special)
    {
        return {};
    }

    switch (character)
    {
    case ARROW_UP:
        return {.y = -1};
    case ARROW_DOWN:
        return {.y = 1};
    case ARROW_RIGHT:
        return {.x = 1};
    case ARROW_LEFT:
        return {.x = -1};
    default:
        return {};
    }
}

void console::keyboard::keyboard::keyboard_input_listener()
{
    while (true)
    {
        const key pressed_key = get_pressed_key_from_stdin();

        // Push the event to the queue after it unlocks
        std::lock_guard lock(keyboard_input_queue_mutex);
        keyboard_input_queue.push(pressed_key);
        keyboard_input_condition_variable.notify_one(); // Allow ::get_key() to continue running
    }
    // ReSharper disable once CppDFAUnreachableCode
}

void set_terminal_non_blocking_mode()
{
    termios terminal{};
    tcgetattr(STDIN_FILENO, &terminal);

    // Disable canonical mode and echo
    terminal.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

console::keyboard::keyboard::keyboard()
{
    set_terminal_non_blocking_mode();
}

console::keyboard::key console::keyboard::keyboard::get_pressed_key()
{
    // Wait for the queue mutex to unlock and contain any items
    std::unique_lock lock(keyboard_input_queue_mutex);
    keyboard_input_condition_variable.wait(lock, [this] { return !keyboard_input_queue.empty(); });

    // Consume the first item of the queue
    const auto keyboard_input = keyboard_input_queue.front();
    keyboard_input_queue.pop();

    return keyboard_input;
}

void console::keyboard::keyboard::listen_for_input()
{
    if (!keyboard_input_listener_thread.has_value())
    {
        keyboard_input_listener_thread = std::thread([this] { keyboard_input_listener(); });
    }
}
