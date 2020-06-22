TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lgdi32
QMAKE_LFLAGS += -municode -static

SOURCES += \
        array.cpp \
	bitstring.cpp \
	five.cpp \
	main.cpp

DISTFILES += \
    small.ico \
    main.ico

HEADERS += \
    array.h \
    bitstring.h \
    five.h \
    resource.h

RC_FILE += main.rc
