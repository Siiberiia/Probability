cmake_minimum_required(VERSION 3.16)
project(EpidemicSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(EpidemicSim
        main.cpp
        model.cpp
        simworker.cpp
        mainwindow.cpp
        model.h
        model.cpp
        model.h
        simworker.cpp
        simworker.h
        mainwindow.cpp
        mainwindow.h
)

target_link_libraries(EpidemicSim PRIVATE Qt6::Widgets)