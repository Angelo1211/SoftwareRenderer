cmake_minimum_required(VERSION 3.7)
project(softwareRenderer)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(softwareRenderer src/main.cpp)
target_link_libraries(softwareRenderer ${SDL2_LIBRARIES})

