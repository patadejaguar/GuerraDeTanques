#ifndef SERVER_H
#define SERVER_H

#include "tcpserver.h"
#include "connection.h"
#include "databaseconnection.h"

struct Game
{
        Connection* _creator;
        int         _index_of_terrain;
        bool        _started;
};

class Server : public TcpServer
{
        Q_OBJECT

    public:

                                Server(bool use_ssl, QObject* parent = 0);

        bool                    listen(const QHostAddress &address, quint16 port);

    signals:

        void                    notifyUserLogin(Connection* connection, QString nick, QByteArray avatar);

    protected slots:

        void                    closeConnection();

    protected:

        void                    incomingConnection(int handle);

    private slots:

        void                    sendMessage(QString message);

        void                    loginUser(QString nick);

        void                    logoutUser(UserProperties user);

        void                    createGame(int index_of_terrain);

        void                    closeGame();

        void                    requestcreatedGames();

        void                    playerJoinedGame(QString creator);

        void                    playerLeavedGame();

        void                    gameStarted();

    private:

        //----------------------_att...

        QList<Game>             _games;
        QStringList             _terrains;
        QStringList             _tanks;
        QStringList             _objects;
};

#endif // SERVER_H
