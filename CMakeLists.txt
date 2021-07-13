cmake_minimum_required(VERSION 3.19)
project(CIS22B)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(function_exercises src/function_exercises.cpp)
add_executable(array_function_exercises src/array_function_exercises.cpp)
add_executable(pointers_practice src/pointers_practice.cpp)
