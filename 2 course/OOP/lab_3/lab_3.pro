TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17

SOURCES += main.cpp \
    modulo.cpp

HEADERS += \
    ModuloTest.h \
    UnitTest.h \
    modulo.h

