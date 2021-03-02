GOOGLETEST_DIR = C:/GIT/dev/googletest

include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        ../../spo_lab/lab01/command.h \
        ../../spo_lab/lab01/cpu.h \
        ../../spo_lab/lab01/memory.h \
        ../../spo_lab/lab01/types.h \
        tst_testMemory.h

SOURCES += \
        ../../spo_lab/lab01/command.cpp \
        ../../spo_lab/lab01/cpu.cpp \
        main.cpp
