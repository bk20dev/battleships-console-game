cmake_minimum_required(VERSION 3.29)
project(battleships_console_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(battleships_console_game main.cpp
        components/input.hpp
        components/input.cpp
        components/label.hpp
        components/label.cpp
        components/list.hpp
        components/list.tpp
        components/list.cpp
        console/console.hpp
        console/console.cpp
        console/keyboard.hpp
        console/keyboard.cpp
        console/style.hpp
        console/style.cpp
        core/component.hpp
        core/component.tpp
        core/component.cpp
        core/component_traits.hpp
        core/component_traits.cpp
        core/dimension.hpp
)
