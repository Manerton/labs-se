TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgdi32
QMAKE_LFLAGS += -municode -static

SOURCES += \
    main.cpp \
    stats.cpp \
    stdafx.cpp \
    trenirovka.cpp

RC_FILE += main.rc

DISTFILES += \
    small.ico \
    main.ico \
    uml.qmodel


HEADERS += \
    resource.h \
    stats.h \
    stdafx.h \
    trenirovka.h
