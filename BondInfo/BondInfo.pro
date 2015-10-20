#-------------------------------------------------
#
# Project created by QtCreator 2015-08-20T10:36:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BondInfo
TEMPLATE = app

DESTDIR = ../bin/app

MYBASE += BaseMarket BaseSQL BaseBond
include($$PWD/../bonds.pri)


SOURCES += main.cpp\
        bondinfo.cpp \
    bondhandler.cpp \
    bondpool.cpp \
    model_bonds_info_table.cpp \
    widget_single_info.cpp

HEADERS  += bondinfo.h \
    bondhandler.h \
    bondpool.h \
    model_bonds_info_table.h \
    widget_single_info.h

FORMS    += bondinfo.ui \
    widget_single_info.ui
