#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T17:56:09
#
#-------------------------------------------------

QT       += core gui multimedia
CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AudioPlayer
TEMPLATE = app


SOURCES += main.cpp\
        player.cpp \
    listwidget.cpp

HEADERS  += player.hpp \
    listwidget.hpp

DISTFILES += \
    Icon.rcc \
    Icon.png

RESOURCES += \
    Icon.qrc
