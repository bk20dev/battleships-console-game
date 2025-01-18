#pragma once

#include <unistd.h>
#include <netinet/in.h>
#include <__format/format_functions.h>

namespace network::native_socket
{
    using ::socket;
    using ::bind;
    using ::listen;
    using ::accept;
    using ::recv;
    using ::close;

    using socket_error_code = errno_t;
    using socket_internet_address = sockaddr_in;
    using socket_address = sockaddr;

    int create_tcp_socket();

    void close_socket(int socket_to_close);

    socket_internet_address create_socket_internet_address(int port);

    void bind_address_to_socket(int socket_descriptor, const socket_internet_address& socket_address);

    void listen_for_connections(int socket_descriptor, int max_clients = 1);

    int accept_client_connection(int socket_descriptor);

    int receive(int socket_descriptor, char* buffer, int buffer_size);
}
