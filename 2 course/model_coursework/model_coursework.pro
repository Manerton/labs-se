QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

QMAKE_LFLAGS += -static

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    demo/demo.cpp \
    demo/diod.cpp \
    demo/halfbitsummator.cpp \
    demo/n_semiconductor.cpp \
    demo/p_semiconductor.cpp \
    demo/particle.cpp \
    demo/picturebox.cpp \
    demo/summatorlogic.cpp \
    demo/transistor.cpp \
    demo/twobitsummator.cpp \
    main.cpp \
    mainwindow.cpp \
    teoria.cpp \
    test.cpp

HEADERS += \
    demo/demo.h \
    demo/diod.h \
    demo/halfbitsummator.h \
    demo/n_semiconductor.h \
    demo/p_semiconductor.h \
    demo/particle.h \
    demo/picturebox.h \
    demo/summatorlogic.h \
    demo/transistor.h \
    demo/twobitsummator.h \
    loader.h \
    mainwindow.h \
    teoria.h \
    test.h

FORMS += \
    demo/demo.ui \
    mainwindow.ui \
    teoria.ui \
    test.ui

RC_FILE = res.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    uml.qmodel \
    uml2.qmodel \
    uml3.qmodel
