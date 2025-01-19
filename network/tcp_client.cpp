#include "tcp_client.hpp"

#include "socket_error.hpp"

void network::tcp_client::listener_thread() const
{
    char message_buffer[1024];
    static constexpr int message_buffer_size = sizeof(message_buffer);

    while (is_connected())
    {
        const int bytes_receive = native_socket
            ::receive(client_socket_descriptor, message_buffer, message_buffer_size);

        if (bytes_receive == 0)
        {
            handle_server_disconnected();
            break;
        }

        const std::string received_message(message_buffer, message_buffer_size);
        handle_server_message(received_message);
    }
}

void network::tcp_client::start_client_listener_thread()
{
    client_listener_thread = std::thread([this]
    {
        listener_thread();
    });
}

void network::tcp_client::close_socket(int& socket_descriptor)
{
    if (socket_descriptor != -1)
    {
        native_socket::close_socket(socket_descriptor);
    }
    socket_descriptor = -1;
}

void network::tcp_client::disconnect()
{
    if (!is_connected())
    {
        return;
    }

    if (client_listener_thread && client_listener_thread->joinable())
    {
        client_listener_thread->join();
    }
    client_listener_thread = std::nullopt;

    close_socket(client_socket_descriptor);
}

void network::tcp_client::handle_server_connected() const
{
    if (on_server_connected)
    {
        on_server_connected();
    }
}

void network::tcp_client::handle_server_disconnected() const
{
    if (on_server_disconnected)
    {
        on_server_disconnected();
    }
}

void network::tcp_client::handle_server_message(const std::string& server_message) const
{
    if (on_server_message)
    {
        on_server_message(server_message);
    }
}

bool network::tcp_client::is_connected() const
{
    return client_socket_descriptor != -1;
}

void network::tcp_client::connect_to(const std::string& ip_address, const int port)
{
    if (is_connected())
    {
        return;
    }

    client_socket_descriptor = native_socket::create_tcp_socket();

    try
    {
        auto server_address = native_socket::create_socket_internet_address(ip_address, port);

        native_socket::connect(client_socket_descriptor, server_address);

        handle_server_connected();
    }
    catch (const socket_error&)
    {
        native_socket::close_socket(client_socket_descriptor);
        client_socket_descriptor = -1;

        throw;
    }

    start_client_listener_thread();
}

void network::tcp_client::send_message(const std::string& message_to_send) const
{
    if (!is_connected())
    {
        return;
    }

    native_socket::send(client_socket_descriptor, message_to_send);
}

void network::tcp_client_connection::handle_server_message(const std::string& server_message) const
{
    if (on_message)
    {
        on_message(server_message);
    }
    tcp_client::handle_server_message(server_message);
}

void network::tcp_client_connection::send(const std::string& message_to_send)
{
    send_message(message_to_send);
}
