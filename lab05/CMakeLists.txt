cmake_minimum_required(VERSION 3.25)
project(lab05)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab05 main.cpp mma.h instruction.h paging.h)
