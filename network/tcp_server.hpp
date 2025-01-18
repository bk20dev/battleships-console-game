#pragma once

#include <thread>

#include "native_socket.hpp"
#include "../engine/interfaces/i_connection.hpp"

namespace network
{
    class tcp_server
    {
        std::optional<std::thread> server_listener_thread = std::nullopt;

        int server_socket_descriptor = -1;
        int client_socket_descriptor = -1;

        void listener_thread();
        void start_server_listener_thread();

        static void close_socket(int& socket_descriptor);

        void stop_listening();

    protected:
        virtual void handle_client_connected() const;
        virtual void handle_client_disconnected() const;
        virtual void handle_client_message(const std::string&) const;

    public:
        virtual ~tcp_server() = default;

        std::function<void()> on_client_connected = nullptr;
        std::function<void()> on_client_disconnected = nullptr;
        std::function<void(const std::string&)> on_client_message = nullptr;

        bool is_listening() const;

        void start_listening(int port);
    };

    class tcp_server_connection final : public tcp_server, public engine::i_connection
    {
        void handle_client_message(const std::string& client_message) const override
        {
            if (on_message)
            {
                on_message(client_message);
            }
            tcp_server::handle_client_message(client_message);
        }
    };
}
