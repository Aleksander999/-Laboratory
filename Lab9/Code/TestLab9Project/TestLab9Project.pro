include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

INCLUDEPATH += ../project

HEADERS += \
        ../project/IKeypad.h \
        ../project/ILatch.h \
        ../project/lockcontroller.h \
        tst_general.h

SOURCES += \
        ../project/lockcontroller.cpp \
        main.cpp
