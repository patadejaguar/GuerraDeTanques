#ifndef CONNECTIONTCP_H
#define CONNECTIONTCP_H

#include <QPixmap>
#include "tcpclient.h"
#include "gameglobals.h"

class ConnectionTcp : public TcpClient
{
        Q_OBJECT

    public:

        static ConnectionTcp*   instance();

        inline UserProperties   userProperties(){return _user_properties;}

        //resources

        void                    sendMissingsResources(QStringList terrains, QStringList tanks, QStringList objects);

        //user commands

        void                    sendRegisterUser(QString nick, QString password, QByteArray avatar);

        void                    sendEditUser(QString nick, QString current_password, QString new_password, QByteArray new_avatar);

        void                    sendLoginUser(QString nick, QString password);

        void                    sendLogoutUser();

        //messages

        void                    sendMessage(QString message);

        //messages related with game

        void                    sendCreateGame(int index_of_terrain);

        void                    sendCloseGame();

        void                    sendRequestCreatedGames();

        void                    sendJoinGame(QString creator);

        void                    sendLeaveGame();

        void                    sendGameStarted();

        void                    sendGameOver(int team_win, QMap<PlayerData, QMap<PlayerData, KillsDeaths> > results);

    signals:

        void                    notifyConnectedData(QStringList terrains, QStringList tanks, QStringList objects);

        void                    notifyResourcesAreReady();

        void                    notifyRegisterSuccessful(QString user);

        void                    notifyEditSuccessful(QString user);

        void                    notifyLoginSuccessful(UserProperties user, QList<UserProperties> users_login);

        void                    notifyMessageReceived(QString message);

        void                    notifyLoginUser(UserProperties user);

        void                    notifyLogoutUser(UserProperties user);

        void                    notifyCreateGame(UserProperties user, int index_of_terrain, QString ip);

        void                    notifyCloseGame(QList<UserProperties> user);

        void                    notifyRequestCreatedGames(QByteArray data);

        void                    notifyPlayerJoinedGame(UserProperties user);

        void                    notifyPlayerLeavedGame(QList<UserProperties> users_properties);

        void                    notifyGameStarted(UserProperties creator);

    protected:

        void                    parseData(QDataStream &stream_input);

        void                    parseConnectedData(QDataStream &stream_input);
        void                    parseMissingResources(QDataStream &stream_input);

        void                    parseRegisterUserSuccessful(QDataStream &stream_input);
        void                    parseRegisterUserError(QDataStream &stream_input);

        void                    parseEditUserSuccessful(QDataStream &stream_input);
        void                    parseEditUserError(QDataStream &stream_input);

        void                    parseLoginSucessful(QDataStream &stream_input);
        void                    parseLoginError(QDataStream &stream_input);

        void                    parseMessage(QDataStream &stream_input);

        void                    parseLoginUser(QDataStream &stream_input);

        void                    parseLogoutUser(QDataStream &stream_input);

        //juego
        void                    parseCreateGame(QDataStream &stream_input);

        void                    parseCloseGame(QDataStream &stream_input);

        void                    parseRequestCreatedGames(QDataStream &stream_input);

        void                    parsePlayerJoinedGame(QDataStream &stream_input);

        void                    parsePlayerLeavedGame(QDataStream &stream_input);

        void                    parseGameStarted(QDataStream &stream_input);

    private:

                                ConnectionTcp();

        //----------------------_att...

        UserProperties          _user_properties;

        static ConnectionTcp*   _instance;
};

#endif // CONNECTIONTCP_H
