#include "text.hpp"

std::string utils::repeat_string(const std::string& string_to_repeat, const int times)
{
    std::string repeated_string = "";
    repeated_string.reserve(string_to_repeat.size() * times);

    for (int i = 0; i < times; i++)
    {
        repeated_string += string_to_repeat;
    }

    return repeated_string;
}
