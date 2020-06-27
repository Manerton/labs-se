TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        queue.cpp \
        testcase.cpp

HEADERS += \
    doctest.h \
    list.h \
    list.inl \
    queue.h
