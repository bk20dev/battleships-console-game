#pragma once

namespace network
{
    class tcp_socket
    {
    protected:
        int socket_descriptor = -1;

    public:
        explicit tcp_socket(int socket_descriptor);

        tcp_socket();

        virtual ~tcp_socket();
    };
}
