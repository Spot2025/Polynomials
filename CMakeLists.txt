cmake_minimum_required(VERSION 3.16)
project(Polynomials)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

add_executable(test
        test.cpp
)
target_link_libraries(test gtest_main)