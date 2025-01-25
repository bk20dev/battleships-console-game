#include "socket_error.hpp"

std::string network::socket_error::format_socket_error_message(
    const std::string& message, const native_socket::socket_error_code error_code)
{
    return std::format("{} (error_code={})", message, error_code);
}

network::native_socket::socket_error_code network::socket_error::get_socket_error_code()
{
    return errno;
}

network::socket_error::socket_error(const std::string& message, const native_socket::socket_error_code& error_code):
    std::runtime_error(format_socket_error_message(message, error_code)), socket_error_code(error_code)
{
}

network::socket_error::socket_error(const std::string& message)
    : socket_error(message, get_socket_error_code())
{
}
