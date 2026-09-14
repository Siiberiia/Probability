cmake_minimum_required(VERSION 3.16)
project(RumourSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(RumourSim
        main.cpp
        model.cpp
        simworker.cpp
        mainwindow.cpp
)

target_link_libraries(RumourSim PRIVATE Qt6::Widgets)