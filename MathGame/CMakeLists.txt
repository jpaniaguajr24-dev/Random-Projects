cmake_minimum_required(VERSION 3.16)

project(MathGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "C:/Qt/6.11.0/llvm_mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(MathGame
        main.cpp
)

target_link_libraries(MathGame
        PRIVATE
        Qt6::Widgets
)