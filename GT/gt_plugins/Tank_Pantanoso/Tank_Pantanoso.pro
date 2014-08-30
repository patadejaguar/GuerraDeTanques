#-------------------------------------------------
#
# Project created by QtCreator 2013-02-22T09:04:05
#
#-------------------------------------------------

TARGET = xavier_tank_pantanoso_v1

QT += widgets multimedia

TEMPLATE = lib

CONFIG += plugin

DESTDIR = ../../bin/tanks

SOURCES += tank.cpp \
    skill1.cpp \
    skill2.cpp \
    skill3.cpp \
    singleanimation.cpp \
    myeffect.cpp

HEADERS += tank.h \
    skill1.h \
    skill2.h \
    skill3.h \
    singleanimation.h \
    myeffect.h

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
