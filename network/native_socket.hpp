#pragma once

#include <stdexcept>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <__format/format_functions.h>

namespace network
{
    namespace native_socket
    {
        using ::socket;
        using ::bind;
        using ::listen;
        using ::accept;
        using ::close;

        using socket_error_code = errno_t;
        using socket_internet_address = sockaddr_in;
        using socket_address = sockaddr;

        int create_tcp_socket();

        void close_socket(int socket_to_close);

        socket_internet_address create_socket_internet_address(int port);

        void bind_address_to_socket(int socket_descriptor, const socket_internet_address& socket_address);
    }

    class socket_error final : public std::runtime_error
    {
        static std::string format_socket_error_message(const std::string& message,
                                                       const native_socket::socket_error_code error_code)
        {
            return std::format("{} (error_code={})", message, error_code);
        }

        static native_socket::socket_error_code get_socket_error_code()
        {
            return errno;
        }

    public:
        const int socket_error_code;

        socket_error(const std::string& message, const native_socket::socket_error_code error_code)
            : std::runtime_error(format_socket_error_message(message, error_code)), socket_error_code(error_code)
        {
        }

        explicit socket_error(const std::string& message) : socket_error(message, get_socket_error_code())
        {
        }
    };
}
