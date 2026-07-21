cmake_minimum_required(VERSION 3.15)
project(simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(simulation
src/main.cpp
src/Simulation.cpp
)

target_link_libraries(simulation raylib)