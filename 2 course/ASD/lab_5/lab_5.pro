TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        queue.cpp \
        testcase.cpp \
        tree.cpp

HEADERS += \
    doctest.h \
    queue.h \
    tree.h
