cmake_minimum_required(VERSION 3.16)

project(TetrisQt)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.0/msvc2022_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TetrisQt
        main.cpp
)

target_link_libraries(TetrisQt
        Qt6::Widgets
)