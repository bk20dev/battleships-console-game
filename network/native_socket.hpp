#pragma once

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <__format/format_functions.h>

namespace network::native_socket
{
    using ::socket;
    using ::inet_pton;
    using ::bind;
    using ::listen;
    using ::accept;
    using ::connect;
    using ::recv;
    using ::send;
    using ::close;

    using socket_error_code = errno_t;
    using socket_internet_address = sockaddr_in;
    using socket_address = sockaddr;

    int create_tcp_socket();

    void close_socket(int socket_to_close);

    socket_internet_address create_socket_internet_address(int port);

    socket_internet_address create_socket_internet_address(const std::string& ip_address, int port);

    void bind_address_to_socket(int socket_descriptor, const socket_internet_address& socket_address);

    void listen_for_connections(int socket_descriptor, int max_clients = 1);

    int accept_client_connection(int socket_descriptor);

    void connect(int socket_descriptor, socket_internet_address& server_internet_address);

    int receive(int socket_descriptor, char* buffer, int buffer_size);

    void send(int socket_descriptor, const std::string& message_to_send);
}
