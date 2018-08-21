cmake_minimum_required(VERSION 3.7)
project(softwareRenderer)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
set(SDL2_PATH "C:\\vs_dev_lib\\SDL2-2.0.8")
find_package(SDL2 REQUIRED)
find_package(OpenMP REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} include libs)

file(GLOB source_files "*.h" "*.cpp" "src/*.cpp" "include/*.h" "libs/*.h")
add_executable(softwareRenderer ${source_files})
#-fopt-info-vec-all -xCORE-AVX2 -no-prec-sqrt  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -g  -ffast-math  ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g ")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g ")
target_link_libraries(softwareRenderer ${SDL2_LIBRARY})

if(WIN32)
    add_custom_command(TARGET softwareRenderer POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLL}
    $<TARGET_FILE_DIR:softwareRenderer>)
endif()
