cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME lab10_1b)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

## находим и подключаем Boost
find_package (Boost REQUIRED)
if(NOT Boost_FOUND)
        message(SEND_ERROR "Failed to find boost.")
        return()
else()
        include_directories(${Boost_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME}  ${Boost_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} rt pthread)
