TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        array.cpp \
        btree.cpp \
        main.cpp \
        queue.cpp \
        testcase.cpp

HEADERS += \
    array.h \
    btree.h \
    doctest.h \
    queue.h
