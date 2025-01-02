#pragma once

#include <functional>
#include <optional>

namespace utils
{
    template <std::ranges::range Container>
    std::optional<typename Container::value_type> find_if_or_null(
        const Container& container_to_search,
        const std::function<bool(const typename Container::value_type&)>& predicate);

    template <std::ranges::range Container>
    bool contains(
        const Container& container_to_search,
        const std::function<bool(const typename Container::value_type&)>& predicate);
}

#include "vector.tpp"
