cmake_minimum_required(VERSION 3.15)
project(CPUSchedulingOS)

set(CMAKE_CXX_STANDARD 14)

add_executable(CPUSchedulingOS
        main.cpp readyQueue.cpp process.cpp process.h readyQueue.h)
