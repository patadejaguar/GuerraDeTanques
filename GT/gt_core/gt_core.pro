#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T10:47:00
#
#-------------------------------------------------

QT       += core widgets multimedia

QT       -= gui

TARGET   = ../../bin/gt_core

CONFIG += shared

CONFIG   += debug_and_release
CONFIG(debug, debug|release) {
 win32: TARGET = $$join(TARGET,,,d)
}

TEMPLATE = lib

DEFINES += GT_CORE_LIBRARY

SOURCES += \
    circleofpower.cpp \
    effect.cpp \
    iobject.cpp \
    iskill.cpp \
    itank.cpp \
    randomnumbermannager.cpp \
    configuration.cpp \
    utilities.cpp \
    infobar.cpp \
    circleofpowereffect.cpp

HEADERS +=\
    gt_core_global.h \
    circleofpower.h \
    effect.h \
    gameglobals.h \
    iobject.h \
    iskill.h \
    itank.h \
    randomnumbermannager.h \
    configuration.h \
    utilities.h \
    infobar.h \
    circleofpowereffect.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lxav_sound_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lxav_sound_engined
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lxav_sound_engine

INCLUDEPATH += $$PWD/../../xav/xav_sound_engine
DEPENDPATH += $$PWD/../../xav/xav_sound_engine

RESOURCES += \
    resources/resources.qrc
