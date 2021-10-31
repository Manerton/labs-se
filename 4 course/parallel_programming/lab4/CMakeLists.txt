cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME lab4)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=undefined -fsanitize-undefined-trap-on-error -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SOURCES main.cpp)

## указываем название бинарника
add_executable(${PROJECT_NAME}  ${SOURCES})

## находим и подключаем Boost
find_package (Boost REQUIRED)
if(NOT Boost_FOUND)
        message(SEND_ERROR "Failed to find boost.")
        return()
else()
        include_directories(${Boost_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} pthread)

