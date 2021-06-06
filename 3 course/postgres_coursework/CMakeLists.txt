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
        Controllers/MainWindow.h Controllers/MainWindow.cpp Views/MainWindow.ui
        Models/Database.h Models/Database.cpp
        Models/IRepository.h
        Models/NagruzkaRepository.h Models/NagruzkaRepository.cpp
        Controllers/AuthForm.h Controllers/AuthForm.cpp Views/AuthForm.ui
        Controllers/BuyerForm.h Controllers/BuyerForm.cpp Views/BuyerForm.ui
        Controllers/ManagerForm.h Controllers/ManagerForm.cpp Views/ManagerForm.ui
        Controllers/OperatorForm.h Controllers/OperatorForm.cpp Views/OperatorForm.ui
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
