cmake_minimum_required(VERSION 3.5)

project(test_gcc_warnings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -ftrapv -fsanitize-undefined-trap-on-error -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
add_executable(test_gcc_warnings main.cpp)
