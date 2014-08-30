#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSettings>

#ifdef WIN32
#include <windows.h>
#endif

#include "music.h"

#include "connectiontcp.h"
#include "configuration.h"
#include "resourcemanager.h"

#include "forms.h"
#include "gameglobals.h"
#include "tcpgameconnectionclient.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:

        explicit            MainWindow(QWidget *parent = 0);

        ~MainWindow();

        void                createScreens();

        void                createConnection();

    public slots:

        void                updateScreenResolution();

    private slots:

        //Connection
        void                resourceManagerReady();
        void                connectionError(QString error, bool critical);
        void                connectionRegistered(QString user);
        void                connectionEdited(QString user);
        void                connectionLogin(UserProperties user_properties, QList<UserProperties> users_login);

    private:

        void                createAndSetCursor();

        Ui::MainWindow *    _ui;

        Music*              _music;


};

#endif // MAINWINDOW_H
