#pragma once

#include <thread>

#include "native_socket.hpp"
#include "tcp_socket.hpp"
#include "../engine/interfaces/i_connection.hpp"

namespace network
{
    class tcp_server final : public engine::i_connection
    {
        std::optional<std::thread> server_listener_thread = std::nullopt;

        int server_socket_descriptor = -1;
        int client_socket_descriptor = -1;

        void handle_client_connected() const;
        void handle_client_disconnected() const;
        void handle_client_message(const std::string&) const;

        void listener_thread();
        void start_server_listener_thread();

        static void close_socket(int& socket_descriptor);

        void stop_listening();

    public:
        std::function<void()> on_client_connected = nullptr;
        std::function<void()> on_client_disconnected = nullptr;
        std::function<void(const std::string&)> on_client_message = nullptr;

        bool is_listening() const;

        void start_listening(int port);
    };
}
