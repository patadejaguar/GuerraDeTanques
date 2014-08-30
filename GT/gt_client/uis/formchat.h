#ifndef FORMCHAT_H
#define FORMCHAT_H

#include "form.h"
#include "connectiontcp.h"
#include "userlistmodel.h"
#include "userlistdelegate.h"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

namespace Ui {
    class FormChat;
}

class FormChat : public Form
{
        Q_OBJECT

    public:

        explicit        FormChat(QWidget *parent = 0);

        ~FormChat();

    public slots:

        void            show();

        void            show(UserProperties user_properties, QList<UserProperties> users_login);

        void            addUserToList(UserProperties user_login);

        void            logoutUser(UserProperties user);

    protected:

        bool            eventFilter(QObject *object, QEvent *event);

    private slots:

        void            showMessageReceived(QString message);

        void            createGame(UserProperties user, int index_of_terrain, QString ip);

        void            closeGame(QList<UserProperties> users);

        void            requestCreatedGames(QByteArray data);

        void            playerJoinedGame(UserProperties user);

        void            playerLeavedGame(QList<UserProperties> users_properties);

        void            on__le_message_returnPressed();

        void            on__pb_exit_clicked();

        void            on__pb_create_game_clicked();

        void            on__pb_join_game_clicked();

    private:

        Ui::FormChat*   _ui;

        UserListModel*  _user_list_model;
};

#endif // FORMCHAT_H
