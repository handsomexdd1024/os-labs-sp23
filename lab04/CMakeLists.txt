cmake_minimum_required(VERSION 3.25)
project(scheduler)

set(CMAKE_CXX_STANDARD 23)

add_executable(scheduler main.cpp output.h toy_pcb.h utils.h scheduler.h)
