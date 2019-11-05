#-------------------------------------------------
#
# Project created by QtCreator 2019-09-26T09:17:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = model_lab_2
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    telo.cpp

HEADERS  += mainwindow.h \
    telo.h

FORMS    += mainwindow.ui
