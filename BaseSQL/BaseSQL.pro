#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T10:28:43
#
#-------------------------------------------------

QT       += core  sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BaseSQL
TEMPLATE = lib

#DEFINES += BASESQL_LIBRARY
CONFIG += staticlib


DESTDIR = ../bin/lib

SOURCES += BaseMySql.cpp \
    BaseSqlite.cpp

HEADERS += basesql_global.h \
    BaseMySql.h \
    BaseSqlite.h \
    IDataBase.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
