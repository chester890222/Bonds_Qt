#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T17:21:28
#
#-------------------------------------------------

QT       += core sql

TARGET = BaseBond
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ..bin/lib

LIBS += ..bin/lib/BaseMarket\
        ..bin/lib/BaseSQL

INCLUDEPATH += ../BaseMarket\
               ../BaseSQL\

SOURCES += basebond.cpp \
    bondinfo.cpp \
    bondrealtimeinfo.cpp

HEADERS += basebond.h \
    bondinfo.h \
    bondrealtimeinfo.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
