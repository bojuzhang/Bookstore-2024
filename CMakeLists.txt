### YOU CAN'T MODIFY THE CODE BELOW
cmake_minimum_required(VERSION 3.15.2)
project(BookStore-2024)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/include/)

### YOU CAN"T MODIFY THE CODE ABOVE

file(GLOB_RECURSE main_src src/*.cpp)

add_executable(code ${main_src}) # Add all *.cpp file after src/main.cpp

