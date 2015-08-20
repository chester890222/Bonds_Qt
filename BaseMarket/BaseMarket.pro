#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T15:14:54
#
#-------------------------------------------------

QT       -= gui

TARGET = BaseMarket
TEMPLATE = lib

#DEFINES += BASEMARKET_LIBRARY
CONFIG += staticlib

DESTDIR = ../bin/lib


SOURCES += \
    BaseTDF.cpp \
    BaseWindQuant.cpp

HEADERS +=\
    basemarket_global.h \
    BaseTDF.h \
    TDF/include/TDFAPI.h \
    TDF/include/TDFAPIStruct.h \
    TDF/include/TDFAPIVersion.h \
    BaseWindQuant.h \
    Wind/Quant/WindQuantAPI.h \
    IBaseMarket.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
