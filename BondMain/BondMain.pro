#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T14:05:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BondMain
TEMPLATE = app

DESTDIR = ../bin

include($$PWD/../bonds.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += \
    mainwindow.ui
