#ifndef CONNECTION_H
#define CONNECTION_H

#include "tcpconnectionserver.h"
#include "databaseconnection.h"
#include "serverconfiguration.h"
#include "gameglobals.h"

#include <QBuffer>
#include <QSqlError>

class Connection : public TcpConnectionServer
{
        Q_OBJECT

    public:

        Connection(bool use_ssl = false, int socket_descriptor = -1, QObject *parent=0);

        ~Connection();

        UserProperties          userProperties() const;

        bool                    updateUserProperties();

        bool                    isLogin();

        QString                 getLocation();

        QString                 location() const;

        void                    setLocation(const QString &location);

        void                    logout();

        void                    sendConnectedData(QStringList terrains, QStringList tanks, QStringList objects);

        void                    sendRegisterUserSuccessful(QString nick);
        void                    sendRegisterUserError(QString error_message);

        void                    sendEditUserSuccessful(QString nick);
        void                    sendEditUserError(QString error_message);

        void                    sendLoginSuccessful(QByteArray data);

        void                    sendLoginResponceError(QString error_message);

        void                    sendMessage(QString message);

        void                    sendUserLogin(QByteArray data);

        void                    sendUserLogout(UserProperties user);

        //game
        void                    sendCreateGame(UserProperties user, int index_of_terrain, QString ip);

        void                    sendCloseGame(QList<UserProperties> users);

        void                    sendCreatedGames(QByteArray data);

        void                    sendPlayerJoinedGame(UserProperties user);

        void                    sendPlayerLeavedGame(QList<UserProperties> users_properties);

        void                    sendGameStarted(UserProperties creator);

    signals:

        void                    notifyRequestLoginUser(QString nick);

        void                    notifyLogoutUser(UserProperties user);

        void                    notifyMessageReceived(QString message);

        void                    notifyCreateGame(int index_of_terrain);

        void                    notifyCloseGame();

        void                    notifyRequestCreatedGames();

        void                    notifyJoinedGame(QString creator);

        void                    notifyLeavedGame();

        void                    notifyGameStarted();

    protected slots:

        void                    parseData(QDataStream &stream_input);

        void                    parseMissingResources(QDataStream &stream_input);

        void                    parseRegisterUser(QDataStream &stream_input);

        void                    parseEditUser(QDataStream &stream_input);

        void                    parseLoginUser(QDataStream &stream_input);

        void                    parseLogoutUser();

        void                    parseMessage(QDataStream &stream_input);

        void                    parseCreateGame(QDataStream &stream_input);

        void                    parseCloseGame();

        void                    parseJoinedGame(QDataStream &stream_input);

        void                    parseGameOver(QDataStream &stream_input);

    private:

        //----------------------_att...

        DataBaseConnection*     _data_base_connection;

        bool                    _login;

        QString                 _location;

        //----------------------_user_properties

        UserProperties          _user_properties;

        //---------------------- data acount
        QString                 _nick;
        QString                 _password;

};


#endif // CONNECTION_H
