cmake_minimum_required(VERSION 3.5)

project(lab01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES main.cpp cpu.cpp command.cpp)
set(HEADERS cpu.h command.h memory.h types.h)
add_executable(lab01 ${SOURCES} ${HEADERS})
