TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
SOURCES += main.cpp \
    cpu.cpp \
    memory.cpp

HEADERS += \
    cpu.h \
    memory.h

