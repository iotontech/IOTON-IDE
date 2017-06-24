#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T11:11:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iotonConfig
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QMAKE_LDFLAGS += -L/usr/bin/lib32

SOURCES += main.cpp\
        iotonconfigwindow.cpp \
    iotonconfigproject.cpp

HEADERS  += iotonconfigwindow.h \
    iotonconfigproject.h

FORMS    += iotonconfigwindow.ui

RESOURCES += \
    iotonConfig.qrc \
    style.qrc
