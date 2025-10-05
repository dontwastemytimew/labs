cmake_minimum_required(VERSION 3.29)
project(labs CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(lab1)

add_executable(labs
        lab1/graph.h
        lab1/main.cpp
        lab1/adjlist.h
)
