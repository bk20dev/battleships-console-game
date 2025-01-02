cmake_minimum_required(VERSION 3.29)
project(battleships_console_game)

set(CMAKE_CXX_STANDARD 20)

add_executable(battleships_console_game main.cpp
        components/battleship.hpp
        components/battleship.cpp
        components/battleship_selector.hpp
        components/battleship_selector.cpp
        components/board_designer.hpp
        components/board_designer.cpp
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
        constants/constants.hpp
        core/component.hpp
        core/component.tpp
        core/component.cpp
        core/component_traits.hpp
        core/component_traits.cpp
        core/dimension.hpp
        core/dimension.cpp
        models/battleship.hpp
        models/battleship.cpp
        screens/board_designer_screen.hpp
        screens/board_designer_screen.cpp
        utils/text.hpp
        utils/text.cpp
        utils/vector.hpp
        utils/vector.tpp
)
