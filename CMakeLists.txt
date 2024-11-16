cmake_minimum_required(VERSION 3.29)
project(battleships_console_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(battleships_console_game main.cpp
        console/console.hpp
        console/keyboard.hpp
        core/component.hpp
        core/component.cpp
        core/dimension.hpp)
