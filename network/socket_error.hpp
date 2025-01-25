#pragma once

#include <stdexcept>
#include <string>

#include "native_socket.hpp"

namespace network
{
    class socket_error final : public std::runtime_error
    {
        static std::string format_socket_error_message(
            const std::string& message, native_socket::socket_error_code error_code);

        static native_socket::socket_error_code get_socket_error_code();

    public:
        const int socket_error_code;

        socket_error(const std::string& message, const native_socket::socket_error_code& error_code);

        explicit socket_error(const std::string& message);
    };
}
