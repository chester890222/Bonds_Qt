#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T10:36:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BondInfo
TEMPLATE = app

DESTDIR = ../bin/app

MYBASE += BaseMarket BaseSQL BaseBond
include($$PWD/../bonds.pri)


SOURCES += main.cpp\
        bondinfo.cpp \
    bondhandler.cpp

HEADERS  += bondinfo.h \
    bondhandler.h

FORMS    += bondinfo.ui
