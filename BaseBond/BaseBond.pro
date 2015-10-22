#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T17:21:28
#
#-------------------------------------------------

QT       += core sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BaseBond
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../bin/lib

#DEFINES += BASEBOND_LIBRARY

MYBASE += BaseMarket BaseSQL
include($$PWD/../bonds.pri)


SOURCES += basebond.cpp \
    bondrealtimeinfo.cpp \
    bond.cpp

HEADERS += basebond.h \
    bondrealtimeinfo.h \
    bond.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
