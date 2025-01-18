#include "native_socket.hpp"

#include <unistd.h>

#include "socket_error.hpp"

int network::native_socket::create_tcp_socket()
{
    const int socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_descriptor < 0)
    {
        throw socket_error("Failed to create a TCP socket.");
    }
    return socket_descriptor;
}

void network::native_socket::close_socket(const int socket_to_close)
{
    if (close(socket_to_close) < 0)
    {
        throw socket_error("Failed to close the socket.");
    }
}

network::native_socket::socket_internet_address network::native_socket::create_socket_internet_address(const int port)
{
    return {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY,
    };
}

void network::native_socket::bind_address_to_socket(const int socket_descriptor,
                                                    const socket_internet_address& socket_address)
{
    // ReSharper disable once CppTooWideScopeInitStatement
    const auto address_to_bind = reinterpret_cast<const native_socket::socket_address*>(&socket_address);
    if (bind(socket_descriptor, address_to_bind, sizeof(socket_address)) < 0)
    {
        throw socket_error("Failed to bind a socket");
    }
}
