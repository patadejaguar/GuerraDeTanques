#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T08:18:06
#
#-------------------------------------------------

QT       += core gui widgets multimedia opengl

TARGET = Build_Terrain

DESTDIR = ../bin

TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp \
    dialognewterrain.cpp \
    tool.cpp \
    iobservable.cpp \
    iobserver.cpp \
    tile.cpp \
    drawpoint.cpp \
    scene.cpp \
    view.cpp \
    treearea.cpp \
    terrain.cpp \
    formtextures.cpp \
    formobjects.cpp \
    formobstructions.cpp \
    point.cpp \
    polygon.cpp \

HEADERS  += mainwindow.h \
    grid.h \
    dialognewterrain.h \
    tool.h \
    iobservable.h \
    iobserver.h \
    tile.h \
    drawpoint.h \
    scene.h \
    view.h \
    treearea.h \
    terrain.h \
    formtextures.h \
    formobjects.h \
    formobstructions.h \
    point.h \
    polygon.h \

FORMS    += mainwindow.ui \
    dialognewterrain.ui \
    formtextures.ui \
    formobjects.ui \
    formobstructions.ui

RESOURCES += resources/resources.qrc

RC_FILE = resources/icon.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lxav_sound_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lxav_sound_engined
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lxav_sound_engine

INCLUDEPATH += $$PWD/../../xav/xav_sound_engine
DEPENDPATH += $$PWD/../../xav/xav_sound_engine

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lgt_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lgt_cored
else:unix: LIBS += -L$$PWD/../bin/ -lgt_core

INCLUDEPATH += $$PWD/../gt_core
DEPENDPATH += $$PWD/../gt_core
