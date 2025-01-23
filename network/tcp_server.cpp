#include "tcp_server.hpp"

#include <iostream>

#include "socket_error.hpp"

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
        try
        {
            listener_thread();
        }
        catch (const socket_error& error)
        {
            handle_network_error(error);
        }
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

void network::tcp_server::handle_network_error(const socket_error& network_error)
{
    stop_listening();

    if (on_network_error)
    {
        on_network_error(network_error);
    }
}

network::tcp_server::~tcp_server()
{
    stop_listening();
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

    try
    {
        const auto server_address = native_socket::create_socket_internet_address(port);

        native_socket::bind_address_to_socket(server_socket_descriptor, server_address);

        static constexpr int max_clients = 1;
        native_socket::listen_for_connections(server_socket_descriptor, max_clients);

        start_server_listener_thread();
    }
    catch (const socket_error& network_error)
    {
        handle_network_error(network_error);
    }
}

void network::tcp_server::stop_listening()
{
    if (!is_listening())
    {
        return;
    }

    close_socket(client_socket_descriptor);
    close_socket(server_socket_descriptor);

    if (server_listener_thread && server_listener_thread->joinable())
    {
        server_listener_thread->join();
    }
    server_listener_thread = std::nullopt;
}

void network::tcp_server::send_message(const std::string& message_to_send) const
{
    if (!is_listening())
    {
        return;
    }

    native_socket::send(client_socket_descriptor, message_to_send);
}

void network::tcp_server_connection::handle_client_message(const std::string& client_message) const
{
    if (on_message)
    {
        on_message(client_message);
    }
    tcp_server::handle_client_message(client_message);
}

void network::tcp_server_connection::handle_client_disconnected() const
{
    if (on_disconnect)
    {
        on_disconnect();
    }
    tcp_server::handle_client_disconnected();
}

void network::tcp_server_connection::send(const std::string& message_to_send)
{
    send_message(message_to_send);
}
