#-------------------------------------------------
#
# Project created by QtCreator 2013-09-27T10:15:26
#
#-------------------------------------------------

TARGET = xavier_object_rune_of_live_v1

QT += widgets multimedia

TEMPLATE = lib

CONFIG += plugin

DESTDIR = ../../bin/objects

SOURCES += object.cpp

HEADERS += object.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lxav_sound_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lxav_sound_engined
else:unix: LIBS += -L$$PWD/../../bin/ -lxav_sound_engine

INCLUDEPATH += $$PWD/../../../xav/xav_sound_engine
DEPENDPATH += $$PWD/../../../xav/xav_sound_engine

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/ -lgt_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/ -lgt_cored
else:unix: LIBS += -L$$PWD/../../bin/ -lgt_core

INCLUDEPATH += $$PWD/../../gt_core
DEPENDPATH += $$PWD/../../gt_core

RESOURCES += \
    resources/resources.qrc

