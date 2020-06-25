TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        array.cpp \
        main.cpp \
        object.cpp \
        testcase.cpp \
        unemployed.cpp \
        unemployedelem.cpp

HEADERS += \
    array.h \
    doctest.h \
    object.h \
    unemployed.h \
    unemployedelem.h
