#ifndef FORMJOINGAME_H
#define FORMJOINGAME_H

#include "form.h"
#include "gameglobals.h"
#include "configuration.h"
#include "connectiontcp.h"
#include "tcpgameconnectionclient.h"
#include "gamelistdelegate.h"

#include <QDataStream>
#include <QStringListModel>

struct GameData
{
        UserProperties  _creator;
        int             _index_of_terrain;
        QString         _ip;
};
namespace Ui {
    class FormJoinGame;
}

class FormJoinGame : public Form
{
        Q_OBJECT

    public:

        explicit            FormJoinGame(QWidget *parent = 0);

        ~FormJoinGame();

    public slots:

        void                updateData(QByteArray data);

        void                updateListOfGames();

        void                show();

        void                hide();

        void                show(QByteArray data);

    protected:

        bool                eventFilter(QObject *object, QEvent *event);

    private slots:

        void                logoutUser(UserProperties user);

        void                createGame(UserProperties user, int index_of_terrain, QString ip);

        void                closeGame(QList<UserProperties> users);

        void                gameStarted(UserProperties creator);

        void                gameFull();

        void                gameJoinError(QString error, bool critical);

        void                gameReadyToStart();

        void                gameIsStarted();

        void                on__pb_cancel_clicked();

        void                on__pb_accept_clicked();

    private:

        void                preview(QModelIndex index);

        void                clearLabels();

        Ui::FormJoinGame *  _ui;

        QList<GameData>     _games_data;

        QStringListModel*   _model;
};

#endif // FORMJOINGAME_H
