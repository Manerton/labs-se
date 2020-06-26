TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    list.inl \
    main.cpp \
    object.cpp \
    testcase.cpp \
    unemployed.cpp \
    unemployedelem.cpp


HEADERS += \
    doctest.h \
    list.h \
    object.h \
    unemployed.h \
    unemployedelem.h
