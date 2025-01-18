#include "tcp_server.hpp"

#include <iostream>

void network::tcp_server::handle_client_connected() const
{
    if (on_client_connected)
    {
        on_client_connected();
    }
}

void network::tcp_server::handle_client_disconnected() const
{
    if (on_client_disconnected)
    {
        on_client_disconnected();
    }
}

void network::tcp_server::handle_client_message(const std::string& client_message) const
{
    if (on_client_message)
    {
        on_client_message(client_message);
    }
}

void network::tcp_server::listener_thread()
{
    client_socket_descriptor = native_socket::
        accept_client_connection(server_socket_descriptor);
    handle_client_connected();

    char message_buffer[1024];
    static constexpr int message_buffer_size = sizeof(message_buffer);

    while (is_listening())
    {
        const int bytes_received = native_socket::
            receive(client_socket_descriptor, message_buffer, message_buffer_size);

        if (bytes_received == 0)
        {
            handle_client_disconnected();
            break;
        }

        const std::string received_message(message_buffer, bytes_received);
        handle_client_message(received_message);
    }

    close_socket(client_socket_descriptor);
}

void network::tcp_server::start_server_listener_thread()
{
    server_listener_thread = std::thread([this]
    {
        listener_thread();
    });
}

void network::tcp_server::close_socket(int& socket_descriptor)
{
    if (socket_descriptor != -1)
    {
        native_socket::close_socket(socket_descriptor);
    }
    socket_descriptor = -1;
}

bool network::tcp_server::is_listening() const
{
    return server_socket_descriptor != -1;
}

void network::tcp_server::start_listening(const int port)
{
    if (is_listening())
    {
        return;
    }

    server_socket_descriptor = native_socket::create_tcp_socket();
    const auto server_address = native_socket::create_socket_internet_address(port);

    native_socket::bind_address_to_socket(server_socket_descriptor, server_address);

    static constexpr int max_clients = 1;
    native_socket::listen_for_connections(server_socket_descriptor, max_clients);

    start_server_listener_thread();
}

void network::tcp_server::stop_listening()
{
    if (!is_listening())
    {
        return;
    }

    if (server_listener_thread && server_listener_thread->joinable())
    {
        server_listener_thread->join();
    }
    server_listener_thread = std::nullopt;

    close_socket(server_socket_descriptor);
    close_socket(client_socket_descriptor);
}
