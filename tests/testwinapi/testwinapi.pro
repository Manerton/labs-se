TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgdi32
QMAKE_LFLAGS += -municode -static

SOURCES += \
    stdafx.cpp \
    test.cpp \
    trenirovka.cpp

RC_FILE += test.rc

DISTFILES += \
    small.ico \
    test.aps \
    test.ico


HEADERS += \
    resource.h \
    stdafx.h \
    targetver.h \
    test.h \
    trenirovka.h
