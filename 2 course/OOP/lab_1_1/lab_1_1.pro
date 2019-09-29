TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    Triangle.cpp \
    main.cpp


HEADERS += \
    Triangle.h

