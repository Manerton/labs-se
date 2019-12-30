TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    testcase.cpp \
    triad.cpp \
    vector3d.cpp

HEADERS += \
    doctest.h \
    triad.h \
    vector3d.h

