TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graph_im.cpp \
        main.cpp \
    graph_al.cpp

HEADERS += \
    degree.h \
    graph.h \
    graph_im.h \
    graph_al.h
