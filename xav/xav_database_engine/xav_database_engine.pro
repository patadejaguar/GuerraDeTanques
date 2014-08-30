#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T14:15:04
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET   = ../../../GT/bin/xav_database_engine

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
 win32: TARGET = $$join(TARGET,,,d)
}

CONFIG += shared

TEMPLATE = lib

DEFINES += XAV_DATABASE_ENGINE_LIBRARY

SOURCES += \
    databaseconnection.cpp

HEADERS +=\
        xav_database_engine_global.h \
    databaseconnection.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
