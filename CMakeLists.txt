cmake_minimum_required(VERSION 3.19)
project(CIS22B)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(src)

add_executable(function_exercises src/function_exercises.cpp)
add_executable(array_function_exercises src/array_function_exercises.cpp)
add_executable(pointers_practice src/pointers_practice.cpp)
add_executable(track_expenses src/track_expenses.cpp)
add_executable(midterm_prac_1 src/midterm_prac_1.cpp)
add_executable(pointers_array src/pointers_array.cpp)
add_executable(writing_classes src/writing_classes.cpp)
