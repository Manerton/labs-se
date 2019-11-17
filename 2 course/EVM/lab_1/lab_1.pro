TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17

SOURCES += main.cpp \
    big_int.cpp \
    testcase.cpp

HEADERS += \
    big_int.h \
    doctest.h

