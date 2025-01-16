#pragma once

#include <functional>

namespace engine
{
    class i_connection
    {
    public:
        std::function<void(const std::string&)> on_message;

        void send(const std::string&);
    };
}
