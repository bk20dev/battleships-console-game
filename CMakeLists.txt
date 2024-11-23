cmake_minimum_required(VERSION 3.29)
project(battleships_console_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(battleships_console_game main.cpp
        console/console.hpp
        console/console.cpp
        console/keyboard.hpp
        console/keyboard.cpp
        core/component.hpp
        core/component.cpp
        core/dimension.hpp
        components/label.cpp
        components/label.hpp
        components/input.hpp
        components/input.cpp
        console/style.hpp
        console/style.cpp
        core/component_traits.hpp
        core/component_traits.cpp)
