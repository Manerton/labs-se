TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../staticlib/release/ -lstaticlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../staticlib/debug/ -lstaticlib

INCLUDEPATH += $$PWD/../staticlib
DEPENDPATH += $$PWD/../staticlib
