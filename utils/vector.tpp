#pragma once

#include "vector.hpp"

template <std::ranges::range Container>
std::optional<typename Container::value_type> utils::find_if_or_null(
    const Container& container_to_search, const std::function<bool(const typename Container::value_type&)>& predicate)
{
    const auto result_iterator = std::find_if(container_to_search.begin(), container_to_search.end(), predicate);

    if (result_iterator != container_to_search.end())
    {
        return *result_iterator;
    }
    return std::nullopt;
}

template <std::ranges::range Container>
bool utils::contains(
    const Container& container_to_search, const std::function<bool(const typename Container::value_type&)>& predicate)
{
    const auto result_iterator = std::find_if(container_to_search.begin(), container_to_search.end(), predicate);
    return result_iterator != container_to_search.end();
}
