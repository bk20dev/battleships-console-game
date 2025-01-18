#pragma once

#include <functional>

namespace engine
{
    class i_connection
    {
    public:
        virtual ~i_connection() = default;

        std::function<void(const std::string&)> on_message;

        virtual void send(const std::string&);
    };
}
