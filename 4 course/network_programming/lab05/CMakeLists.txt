cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME lab05)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.cpp
    Sniffer.h
    WinSniffer.h WinSniffer.cpp
    LinuxSniffer.h LinuxSniffer.cpp
    PacketParser.hpp
    )

target_link_libraries(${PROJECT_NAME} pthread)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()
