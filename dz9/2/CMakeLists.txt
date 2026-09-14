cmake_minimum_required(VERSION 3.16)
project(RandomWalk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(RandomWalk
        main.cpp
        distribution.cpp
        walkwidget.cpp
        mainwindow.cpp
)

target_link_libraries(RandomWalk PRIVATE Qt6::Widgets)