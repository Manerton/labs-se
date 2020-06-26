TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        array.inl \
        main.cpp \
        object.cpp \
        testcase.cpp \
        unemployed.cpp \
        unemployedelem.cpp

HEADERS += \
    array.h \
    doctest.h \
    object.h \
    queue1.h \
    queue2.h \
    unemployed.h \
    unemployedelem.h
