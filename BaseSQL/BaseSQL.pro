#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T11:15:02
#
#-------------------------------------------------

QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BaseSQL
TEMPLATE = lib
CONFIG += staticlib

DESTDIR = ../bin/lib

SOURCES += basemysql.cpp

HEADERS += basemysql.h \
    IDataBase.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
