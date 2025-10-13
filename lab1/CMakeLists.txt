cmake_minimum_required(VERSION 3.29)
project(lab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)


add_executable(tests
        tests/main_tests.cpp
)


target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
