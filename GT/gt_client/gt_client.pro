#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T09:04:45
#
#-------------------------------------------------

QT       += core gui network widgets multimedia opengl

TARGET = GT_Client

DESTDIR = ../bin

TEMPLATE = app

INCLUDEPATH +=  uis\
                game\
                network\

SOURCES += main.cpp\
        uis/mainwindow.cpp \
        uis/formprincipal.cpp \
        uis/formconnectedtoserver.cpp \
        uis/formcreateuseraccount.cpp \
        uis/form.cpp \
        uis/formloadavatar.cpp \
        uis/formedituseraccount.cpp \
        uis/filterproxymodel.cpp \
        uis/formchat.cpp \
        uis/userlistmodel.cpp \
        uis/userlistdelegate.cpp \
        uis/formcreategame.cpp \
        uis/formcreatedgame.cpp \
        uis/formjoingame.cpp \
        uis/formdialogmessage.cpp \
        uis/formtankselector.cpp \
        uis/forms.cpp \
        uis/formbeta.cpp \
        uis/terrainlistdelegate.cpp \
        uis/gamelistdelegate.cpp \
        uis/formgame.cpp \
        uis/formconfiguration.cpp \
        uis/formcontrols.cpp \
        uis/formsound.cpp \
        uis/formnetwork.cpp \
        uis/forminitgame.cpp \
        network/connectiontcp.cpp \
        network/tcpgameconnectionclient.cpp \
        network/tcpgameconnectionserver.cpp \
        network/tcpgameserver.cpp \
        game/game.cpp \
        game/player.cpp \
        game/team.cpp \
        game/music.cpp \
        game/terrain.cpp \
    game/resourcemanager.cpp \
    uis/formresults.cpp \
    uis/fadelabeleffect.cpp \
    uis/formhelp.cpp \
    uis/formreplayselect.cpp \
    uis/formcredits.cpp \
    uis/formreplayview.cpp \
    uis/dialogshorcut.cpp \
    uis/dialog.cpp

HEADERS  += uis/mainwindow.h \
        uis/formprincipal.h \
        uis/formconnectedtoserver.h \
        uis/formcreateuseraccount.h \
        uis/form.h \
        uis/formloadavatar.h \
        uis/formedituseraccount.h \
        uis/filterproxymodel.h \
        uis/formchat.h \
        uis/userlistmodel.h \
        uis/userlistdelegate.h \
        uis/formcreategame.h \
        uis/formcreatedgame.h \
        uis/formjoingame.h \
        uis/formdialogmessage.h \
        uis/formtankselector.h \
        uis/forms.h \
        uis/formbeta.h \
        uis/formgame.h \
        uis/formconfiguration.h \
        uis/formcontrols.h \
        uis/formsound.h \
        uis/formnetwork.h \
        uis/forminitgame.h \
        uis/terrainlistdelegate.h \
        uis/gamelistdelegate.h \
        network/connectiontcp.h \
        network/tcpgameconnectionclient.h \
        network/tcpgameconnectionserver.h \
        network/tcpgameserver.h \
        game/game.h \
        game/player.h \
        game/team.h \
        game/music.h \
        game/terrain.h \
    game/resourcemanager.h \
    uis/formresults.h \
    uis/fadelabeleffect.h \
    uis/formhelp.h \
    uis/formreplayselect.h \
    uis/formcredits.h \
    uis/formreplayview.h \
    uis/dialogshorcut.h \
    uis/dialog.h

FORMS    += uis/mainwindow.ui \
        uis/formprincipal.ui \
        uis/formconnectedtoserver.ui \
        uis/formcreateuseraccount.ui \
        uis/formloadavatar.ui \
        uis/formedituseraccount.ui \
        uis/formchat.ui \
        uis/formcreategame.ui \
        uis/formcreatedgame.ui \
        uis/formjoingame.ui \
        uis/formtankselector.ui \
        uis/formdialogmessage.ui \
        uis/formbeta.ui \
        uis/formgame.ui \
        uis/formconfiguration.ui \
        uis/formcontrols.ui \
        uis/formsound.ui \
        uis/formnetwork.ui \
        uis/forminitgame.ui \
    uis/formresults.ui \
    uis/formhelp.ui \
    uis/formreplayselect.ui \
    uis/formcredits.ui \
    uis/formreplayview.ui \
    uis/dialogshorcut.ui \
    uis/dialog.ui

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/ -lxav_network_engine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/ -lxav_network_engined
else:unix:!macx: LIBS += -L$$PWD/../bin/ -lxav_network_engine

INCLUDEPATH += $$PWD/../../xav/xav_network_engine
DEPENDPATH += $$PWD/../../xav/xav_network_engine


