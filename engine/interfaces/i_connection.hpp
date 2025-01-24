#pragma once

#include <functional>

namespace engine
{
    class i_connection
    {
    public:
        virtual ~i_connection() = default;

        std::function<void(const std::string&)> on_receive;
        std::function<void()> on_disconnect;

        virtual void send(const std::string&);
    };
}
