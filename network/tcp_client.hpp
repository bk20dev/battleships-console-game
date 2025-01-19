#pragma once

#include <functional>
#include <string>
#include <thread>

#include "../engine/interfaces/i_connection.hpp"

namespace network
{
    class tcp_client
    {
        std::optional<std::thread> client_listener_thread = std::nullopt;

        int client_socket_descriptor = -1;

        void listener_thread() const;
        void start_client_listener_thread();

        static void close_socket(int& socket_descriptor);

        void disconnect();

    protected:
        virtual void handle_server_connected() const;
        virtual void handle_server_disconnected() const;
        virtual void handle_server_message(const std::string&) const;

    public:
        virtual ~tcp_client() = default;

        std::function<void()> on_server_connected = nullptr;
        std::function<void()> on_server_disconnected = nullptr;
        std::function<void(const std::string&)> on_server_message = nullptr;

        bool is_connected() const;

        void connect_to(const std::string& ip_address, int port);

        void send_message(const std::string&) const;
    };

    class tcp_client_connection final : public tcp_client, public engine::i_connection
    {
        void handle_server_message(const std::string&) const override;

    public:
        void send(const std::string&) override;
    };
}
