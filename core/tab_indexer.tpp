#pragma once

template <typename T>
std::shared_ptr<T> core::tab_indexer::get_focused_component()
{
    const auto& component_at_index = get_component_at_index(focused_component_index);
    return std::dynamic_pointer_cast<T>(component_at_index);
}
