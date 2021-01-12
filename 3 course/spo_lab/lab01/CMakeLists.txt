cmake_minimum_required(VERSION 3.5)

project(lab01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES main.cpp cpu.cpp command.cpp)
set(HEADERS cpu.h command.h memory.h types.h)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -fsanitize-undefined-trap-on-error -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
add_executable(lab01 ${SOURCES} ${HEADERS})
