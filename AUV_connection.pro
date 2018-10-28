TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    serial.cpp \
    messages.cpp

HEADERS += \
    serial.h \
    messages.h
