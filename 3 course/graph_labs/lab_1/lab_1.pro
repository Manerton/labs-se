TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        graph_am.cpp \
        graph_el.cpp \
        graph_im.cpp \
        main.cpp

HEADERS += \
    degree.h \
    graph.h \
    graph_am.h \
    graph_el.h \
    graph_im.h
