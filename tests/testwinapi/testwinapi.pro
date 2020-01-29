TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_LFLAGS += -municode

SOURCES += \
    stdafx.cpp \
    test.cpp

RC_FILE += test.rc

DISTFILES += \
    small.ico \
    test.aps \
    test.ico


HEADERS += \
    resource.h \
    stdafx.h \
    targetver.h \
    test.h
