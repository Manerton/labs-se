cmake_minimum_required(VERSION 3.5)

project(lab03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(HEADERS cpu.h command.h memory.h types.h debugger.h)
set(SOURCES main.cpp cpu.cpp command.cpp debugger.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -fsanitize-undefined-trap-on-error -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
add_executable(lab03 ${SOURCES} ${HEADERS})
