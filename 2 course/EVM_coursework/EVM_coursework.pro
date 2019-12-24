TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17
SOURCES += main.cpp \
    command.cpp \
    cpu.cpp \
    memory.cpp

HEADERS += \
    command.h \
    cpu.h \
    memory.h \
    types.h

DISTFILES += \
    uml.qmodel

