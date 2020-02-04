TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgdi32
QMAKE_LFLAGS += -municode -static

SOURCES += \
    main.cpp \
    stdafx.cpp \
    trenirovka.cpp

RC_FILE += test.rc

DISTFILES += \
    small.ico \
    test.aps \
    test.ico


HEADERS += \
    main.h \
    resource.h \
    stdafx.h \
    trenirovka.h
