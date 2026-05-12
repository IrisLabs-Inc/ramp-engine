cmake_minimum_required(VERSION 3.20)

project(ramp-engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED CONFIG)

add_executable(ramp-engine
	src/main.cpp
)

target_link_libraries(ramp-engine PRIVATE SDL3::SDL3)
