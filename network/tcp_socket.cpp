#include "tcp_socket.hpp"

#include "native_socket.hpp"

network::tcp_socket::tcp_socket(const int socket_descriptor)
    : socket_descriptor(socket_descriptor)
{
}

network::tcp_socket::tcp_socket()
    : tcp_socket(native_socket::create_tcp_socket())
{
}

network::tcp_socket::~tcp_socket()
{
    native_socket::close_socket(socket_descriptor);
    socket_descriptor = -1;
}
