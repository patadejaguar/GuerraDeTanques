#-------------------------------------------------
#
# Project created by QtCreator 2013-01-30T08:14:08
#
#-------------------------------------------------

QT       += core gui network sql widgets

INCLUDEPATH += ../gt_network\
               uis\
               core

DESTDIR = ../bin

TARGET = GT_Server

TEMPLATE = app

SOURCES += main.cpp \
    serverconfiguration.cpp \
    core/server.cpp \
    core/connection.cpp \
    uis/serverstatus.cpp \
    uis/mainwindow.cpp \
    uis/dialogquery.cpp \
    uis/dialogconfiguration.cpp \
    uis/connectionmodel.cpp \
    uis/avatardelegate.cpp \
    uis/connectiondelegate.cpp \
    uis/passworddelegate.cpp

HEADERS += \
    serverconfiguration.h \
    core/server.h \
    core/connection.h \
    uis/serverstatus.h \
    uis/mainwindow.h \
    uis/dialogquery.h \
    uis/dialogconfiguration.h \
    uis/connectionmodel.h \
    uis/avatardelegate.h \
    uis/connectiondelegate.h \
    uis/passworddelegate.h

FORMS += \
    uis/serverstatus.ui \
    uis/mainwindow.ui \
    uis/dialogquery.ui \
    uis/dialogconfiguration.ui \
    uis/connectionitemview.ui

RESOURCES += resources/resources.qrc

RC_FILE = resources/icon.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lxav_database_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lxav_database_engined
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lxav_database_engine

INCLUDEPATH += $$PWD/../../xav/xav_database_engine
DEPENDPATH += $$PWD/../../xav/xav_database_engine

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lxav_network_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lxav_network_engined
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lxav_network_engine

INCLUDEPATH += $$PWD/../../xav/xav_network_engine
DEPENDPATH += $$PWD/../../xav/xav_network_engine

INCLUDEPATH += $$PWD/../gt_core
DEPENDPATH += $$PWD/../gt_core
