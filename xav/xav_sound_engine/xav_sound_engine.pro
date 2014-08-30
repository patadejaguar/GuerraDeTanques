#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T13:27:53
#
#-------------------------------------------------

QT       += multimedia

QT       -= gui

TARGET   = ../../../GT/bin/xav_sound_engine

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
 win32: TARGET = $$join(TARGET,,,d)
}

CONFIG += shared

TEMPLATE = lib

DEFINES += XAV_SOUND_ENGINE_LIBRARY

SOURCES += \
    soundengine.cpp

HEADERS += xav_sound_engine_global.h \
    soundengine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
