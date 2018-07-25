cmake_minimum_required(VERSION 3.7)
project(softwareRenderer)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
#set(SDL2_PATH "C:\\vs_dev_lib\\SDL2-2.0.8")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} include libs)

file(GLOB source_files "*.h" "*.cpp" "src/*.cpp" "include/*.h" "libs/*.h")
message(STATUS $ENV{source_files})
add_executable(softwareRenderer ${source_files})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ")
target_link_libraries(softwareRenderer ${SDL2_LIBRARY})

if(WIN32)
    add_custom_command(TARGET softwareRenderer POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLL}
    $<TARGET_FILE_DIR:softwareRenderer>)
endif()
