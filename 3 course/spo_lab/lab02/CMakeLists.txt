cmake_minimum_required(VERSION 3.5)

project(lab02 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(HEADERS types.h translator.h operatorparser.h mathexpression.h mathparser.h)
set(SOURCES main.cpp translator.cpp operatorparser.cpp mathparser.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -fsanitize-undefined-trap-on-error -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
add_executable(lab02 ${SOURCES} ${HEADERS})
