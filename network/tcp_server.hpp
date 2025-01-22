#pragma once

#include <thread>

#include "native_socket.hpp"
#include "socket_error.hpp"
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

    protected:
        virtual void handle_client_connected() const;
        virtual void handle_client_disconnected() const;
        virtual void handle_client_message(const std::string&) const;
        virtual void handle_network_error(const socket_error&) const;

    public:
        virtual ~tcp_server();

        std::function<void()> on_client_connected = nullptr;
        std::function<void()> on_client_disconnected = nullptr;
        std::function<void(const std::string&)> on_client_message = nullptr;
        std::function<void(const socket_error&)> on_network_error = nullptr;

        [[nodiscard]] bool is_listening() const;

        void start_listening(int port);
        void stop_listening();

        void send_message(const std::string&) const;
    };

    class tcp_server_connection final : public tcp_server, public engine::i_connection
    {
        void handle_client_message(const std::string& client_message) const override;

    public:
        void send(const std::string&) override;
    };
}
