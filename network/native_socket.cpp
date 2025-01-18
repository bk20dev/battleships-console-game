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
    if (port < 0 || port >= 65536)
    {
        throw socket_error("Port out of bounds", 0);
    }

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

void network::native_socket::listen_for_connections(const int socket_descriptor, const int max_clients)
{
    if (listen(socket_descriptor, max_clients) < 0)
    {
        throw socket_error("Failed to listen on the socket.");
    }
}

int network::native_socket::accept_client_connection(const int socket_descriptor)
{
    const int client_socket = accept(socket_descriptor, nullptr, nullptr);
    if (client_socket < 0)
    {
        throw socket_error("Failed to accept client connection.");
    }
    return client_socket;
}

int network::native_socket::receive(const int socket_descriptor, char* const buffer, const int buffer_size)
{
    constexpr int flags = 0;
    const int bytes_read = recv(socket_descriptor, buffer, buffer_size, flags);
    if (bytes_read < 0)
    {
        throw socket_error("Failed to read from socket");
    }
    return bytes_read;
}
