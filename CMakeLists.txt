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
        core/dimension.hpp)
