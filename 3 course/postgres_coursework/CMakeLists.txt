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
        Models/Errors.h
        Models/ManagerRepository.h Models/ManagerRepository.cpp
        Models/CategoryRepository.h Models/CategoryRepository.cpp
        Models/ManufacturerRepository.h Models/ManufacturerRepository.cpp
        Models/DeliveryPointRepository.h Models/DeliveryPointRepository.cpp
        Models/ProductRepository.h Models/ProductRepository.cpp
        Models/ProductInWarehouseRepository.h Models/ProductInWarehouseRepository.cpp
        Models/ClientRepository.h Models/ClientRepository.cpp
        Models/OrderRepository.h Models/OrderRepository.cpp
        Models/CustomProxyModel.h
        Controllers/AuthForm.h Controllers/AuthForm.cpp Views/AuthForm.ui
        Controllers/BuyerForm.h Controllers/BuyerForm.cpp Views/BuyerForm.ui
        Controllers/ManagerForm.h Controllers/ManagerForm.cpp Views/ManagerForm.ui
        Controllers/OperatorForm.h Controllers/OperatorForm.cpp Views/OperatorForm.ui
        Controllers/CartForm.h Controllers/CartForm.cpp Views/CartForm.ui
        Controllers/CreateOrderForm.h Controllers/CreateOrderForm.cpp Views/CreateOrderForm.ui
        Controllers/SmsConfirmDialog.h Controllers/SmsConfirmDialog.cpp Views/SmsConfirmDialog.ui
        Controllers/OrderDetailsForm.h Controllers/OrderDetailsForm.cpp Views/OrderDetailsForm.ui
        StringTools.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(postgres_coursework
        ${PROJECT_SOURCES}
    )
else()
        add_executable(postgres_coursework WIN32
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(postgres_coursework PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)
