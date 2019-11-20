#-------------------------------------------------
#
# Project created by QtCreator 2019-09-26T09:17:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = model_lab_3
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17

SOURCES += main.cpp\
        animal.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    animal.h

FORMS    += mainwindow.ui
