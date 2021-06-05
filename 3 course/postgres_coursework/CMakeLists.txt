cmake_minimum_required(VERSION 3.5)

project(postgres_coursework LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)
set(PROJECT_SOURCES
        main.cpp
        mainwindow.ui MainWindow.h MainWindow.cpp
        Database.h Database.cpp
        IRepository.h
        NagruzkaRepository.h NagruzkaRepository.cpp
        AuthForm.h AuthForm.cpp AuthForm.ui
        OperatorForm.h OperatorForm.cpp OperatorForm.ui
        StringTools.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(postgres_coursework
        ${PROJECT_SOURCES}
    )
else()
        add_executable(postgres_coursework
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(postgres_coursework PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
