#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T13:52:38
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET   = ../../../GT/bin/xav_network_engine

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
 win32: TARGET = $$join(TARGET,,,d)
}

CONFIG += shared

TEMPLATE = lib

DEFINES += XAV_NETWORK_ENGINE_LIBRARY

SOURCES += tcpclient.cpp \
    tcpconnection.cpp \
    tcpconnectionserver.cpp \
    tcpserver.cpp

HEADERS += xav_network_engine_global.h \
    tcpclient.h \
    tcpconnection.h \
    tcpconnectionserver.h \
    tcpserver.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
