TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../dll/release/ -ldll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../dll/debug/ -ldll

INCLUDEPATH += $$PWD/../dll
DEPENDPATH += $$PWD/../dll
