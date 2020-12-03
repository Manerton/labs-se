QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    haffman.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    shenonfano.cpp

HEADERS += \
    haffman.h \
    mainwindow.h \
    model.h \
    shenonfano.h \
    svgview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

unix|win32: LIBS += -L$$PWD/lib/ -lQtSvgView

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/QtSvgView.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/libQtSvgView.a
