TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        hash.cpp \
        main.cpp \
        queue.cpp \
        testcase.cpp

HEADERS += \
    doctest.h \
    hash.h \
    list.h \
    list.inl \
    queue.h
