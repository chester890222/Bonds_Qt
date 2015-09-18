#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T11:10:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GBFcalculator
TEMPLATE = app

DESTDIR = ../bin/app

MYBASE += BaseMarket BaseSQL BaseBond
include($$PWD/../bonds.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    basegbf.cpp

HEADERS  += mainwindow.h \
    basegbf.h

FORMS    += mainwindow.ui
