#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T17:21:28
#
#-------------------------------------------------

QT       += core sql

TARGET = BaseBond
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../bin/lib

MYBASE += BaseMarket BaseSQL
include($$PWD/../bonds.pri)
#LIBS += ../bin/lib/BaseMarket.lib\
#        ../bin/lib/BaseSQL.lib


#INCLUDEPATH += ../BaseMarket\
#               ../BaseSQL\


SOURCES += basebond.cpp \
    bondrealtimeinfo.cpp

HEADERS += basebond.h \
    bondrealtimeinfo.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
