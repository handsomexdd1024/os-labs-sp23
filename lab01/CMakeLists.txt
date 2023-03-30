cmake_minimum_required(VERSION 3.24)
project(lab01 C)

set(CMAKE_C_STANDARD 11)

add_executable(pid pid.c)
add_executable(exec exec.c)
add_executable(test test.c)
