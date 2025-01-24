#include "tcp_client.hpp"

#include "socket_error.hpp"

void network::tcp_client::listener_thread() const
{
    char message_buffer[1024];
    static constexpr int message_buffer_size = sizeof(message_buffer);

    while (is_listening())
    {
        const int bytes_received = native_socket::
            receive(client_socket_descriptor, message_buffer, message_buffer_size);

        if (bytes_received == 0)
        {
            handle_server_disconnected();
            break;
        }

        const std::string received_message(message_buffer, bytes_received);
        handle_server_message(received_message);
    }
}

void network::tcp_client::start_client_listener_thread()
{
    client_listener_thread = std::thread([this]
    {
        try
        {
            listener_thread();
        }
        catch (const socket_error& network_error)
        {
            handle_network_error(network_error);
        }
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

void network::tcp_client::handle_network_error(const socket_error& network_error)
{
    disconnect();

    if (on_network_error)
    {
        on_network_error(network_error);
    }
}

network::tcp_client::~tcp_client()
{
    disconnect();
}

bool network::tcp_client::is_listening() const
{
    return client_socket_descriptor != -1;
}

void network::tcp_client::connect_to(const std::string& ip_address, const int port)
{
    if (is_listening())
    {
        return;
    }

    client_socket_descriptor = native_socket::create_tcp_socket();

    try
    {
        auto server_address = native_socket::create_socket_internet_address(ip_address, port);

        native_socket::connect(client_socket_descriptor, server_address);
        handle_server_connected();

        start_client_listener_thread();
    }
    catch (const socket_error& network_error)
    {
        handle_network_error(network_error);
    }
}

void network::tcp_client::disconnect()
{
    if (!is_listening())
    {
        return;
    }

    close_socket(client_socket_descriptor);

    if (client_listener_thread && client_listener_thread->joinable())
    {
        client_listener_thread->join();
    }
    client_listener_thread = std::nullopt;
}

void network::tcp_client::send_message(const std::string& message_to_send) const
{
    if (!is_listening())
    {
        return;
    }

    native_socket::send(client_socket_descriptor, message_to_send);
}

void network::tcp_client_connection::handle_server_message(const std::string& server_message) const
{
    if (on_receive)
    {
        on_receive(server_message);
    }
    tcp_client::handle_server_message(server_message);
}

void network::tcp_client_connection::handle_server_disconnected() const
{
    if (on_disconnect)
    {
        on_disconnect();
    }
    tcp_client::handle_server_disconnected();
}

void network::tcp_client_connection::send(const std::string& message_to_send)
{
    send_message(message_to_send);
}
