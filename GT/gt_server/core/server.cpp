#include "server.h"

Server::Server(bool use_ssl, QObject *parent):TcpServer(use_ssl,parent)
{
}

bool Server::listen(const QHostAddress &address, quint16 port)
{
    _terrains = DIR_TERRAINS.entryList(QStringList()<<"*.gmm", QDir::Files);
    _tanks = DIR_TANKS.entryList(QStringList()<<"*.dll", QDir::Files);;
    _objects = DIR_OBJECTS.entryList(QStringList()<<"*.dll", QDir::Files);
    return QTcpServer::listen(address, port);
}

void Server::closeConnection()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;
    Connection *connection_sender = (Connection*)sender_object;

    if(connection_sender->isLogin())
    {
        foreach(TcpConnection* connection, _tcp_connections)
            if(((Connection*)connection)->location() == TCP_LOCATION_IN_CHAT)
                ((Connection*)connection)->sendUserLogout(connection_sender->userProperties());

        for(int i = 0; i < _games.size(); ++i)
        {
            if(_games.at(i)._creator == connection_sender)
            {
                _games.removeAt(i);
                i = _games.size();
            }
        }
    }

    TcpServer::closeConnection(connection_sender);
}

void Server::incomingConnection(int handle)
{
    Connection *connection = new Connection(_use_ssl, handle);

    connect(connection, &Connection::notifyDisconnected, this, &Server::closeConnection);
    connect(connection, &Connection::notifyMessageReceived, this, &Server::sendMessage);
    connect(connection, &Connection::notifyRequestLoginUser, this, &Server::loginUser);
    connect(connection, &Connection::notifyLogoutUser, this, &Server::logoutUser);
    //game
    connect(connection, &Connection::notifyCreateGame, this, &Server::createGame);
    connect(connection, &Connection::notifyCloseGame, this, &Server::closeGame);
    connect(connection, &Connection::notifyRequestCreatedGames, this, &Server::requestcreatedGames);
    connect(connection, &Connection::notifyJoinedGame, this, &Server::playerJoinedGame);
    connect(connection, &Connection::notifyLeavedGame, this, &Server::playerLeavedGame);
    connect(connection, &Connection::notifyGameStarted, this, &Server::gameStarted);

    addConnection(connection);
    connection->sendConnectedData(_terrains, _tanks, _objects);
}

void Server::sendMessage(QString message)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;

    foreach(TcpConnection* connection, _tcp_connections)
        if(((Connection*)connection)->location() == connection_sender->location())
            ((Connection*)connection)->sendMessage(message);
}

void Server::loginUser(QString nick)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;

    foreach (TcpConnection* tcp_connection, _tcp_connections)
    {
        Connection* connection = (Connection*)tcp_connection;
        if(connection->userProperties()._nick == nick)
        {
            connection_sender->sendLoginResponceError(tr("Este usuario ya está conectado desde: %1.").arg(connection->address()));
            return;
        }
    }

    if(!connection_sender->updateUserProperties())
        return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << connection_sender->userProperties();

    foreach(TcpConnection* connection, _tcp_connections)
    {
        if(((Connection*)connection)->location() == TCP_LOCATION_IN_CHAT)
        {
            stream << ((Connection*)connection)->userProperties();

            QByteArray data_connection;
            QDataStream stream_connection(&data_connection, QIODevice::WriteOnly);
            stream_connection << connection_sender->userProperties();
            ((Connection*)connection)->sendUserLogin(data_connection);
        }
    }

    connection_sender->setLocation(TCP_LOCATION_IN_CHAT);
    connection_sender->sendLoginSuccessful(data);
}

void Server::logoutUser(UserProperties user)
{
    foreach(TcpConnection* connection, _tcp_connections)
        if(((Connection*)connection)->isLogin())
        {
            if(((Connection*)connection)->userProperties() == user)
                ((Connection*)connection)->logout();
            ((Connection*)connection)->sendUserLogout(user);
        }
}

void Server::createGame(int index_of_terrain)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;
    Connection *connection_sender = (Connection*)sender_object;

    Game game;
    game._started = false;
    game._creator = connection_sender;
    game._index_of_terrain = index_of_terrain;

    _games.append(game);

    foreach(TcpConnection *connection, _tcp_connections)
        if(((Connection*)connection)->location() == TCP_LOCATION_IN_CHAT)
            ((Connection*)connection)->sendCreateGame(connection_sender->userProperties(), index_of_terrain, connection_sender->address());

    connection_sender->setLocation(QString("in_game_of_%1").arg(connection_sender->userProperties()._nick));

}

void Server::closeGame()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;
    connection_sender->updateUserProperties();

    QString nick = connection_sender->userProperties()._nick;
    QList<UserProperties> users_prop;
    foreach(TcpConnection *tcp_connection, _tcp_connections)
    {
        Connection* connection = (Connection*)tcp_connection;
        if(connection->location() == TCP_LOCATION_IN_CHAT)
            users_prop.append(connection->userProperties());
        else if(connection->location() == TCP_LOCATION_IN_GAME.arg(nick))
        {
            connection->setLocation(TCP_LOCATION_IN_CHAT);
            connection->updateUserProperties();
            users_prop.append(connection->userProperties());
        }
    }

    connection_sender->sendPlayerLeavedGame(users_prop);

    foreach(TcpConnection *tcp_connection, _tcp_connections)
    {
        Connection* connection = (Connection*)tcp_connection;
        if(connection->location() == TCP_LOCATION_IN_CHAT)
            connection->sendCloseGame(users_prop);
    }

    connection_sender->setLocation(TCP_LOCATION_IN_CHAT);

    for (int i = 0; i < _games.size(); ++i)
    {
        if(_games.at(i)._creator->userProperties()._nick == nick)
        {
            _games.removeAt(i);
            i = _games.size();
        }
    }
}

void Server::requestcreatedGames()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    for (int i = 0; i < _games.size(); ++i)
    {
        if(!_games.at(i)._started)
        {
            stream<<_games.at(i)._creator->userProperties();
            stream<<_games.at(i)._index_of_terrain;
            stream<<_games.at(i)._creator->address();
        }
    }

    connection_sender->sendCreatedGames(data);
}

void Server::playerJoinedGame(QString creator)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;
    connection_sender->setLocation(TCP_LOCATION_IN_GAME.arg(creator));

    foreach(TcpConnection *connection, _tcp_connections)
        ((Connection*)connection)->sendPlayerJoinedGame(connection_sender->userProperties());
}

void Server::playerLeavedGame()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;
    connection_sender->setLocation(TCP_LOCATION_IN_CHAT);

    QList<UserProperties> users_prop;
    foreach(TcpConnection *connection, _tcp_connections)
        if(((Connection*)connection)->location() == TCP_LOCATION_IN_CHAT)
            users_prop.append(((Connection*)connection)->userProperties());

    foreach(TcpConnection *connection, _tcp_connections)
        ((Connection*)connection)->sendPlayerLeavedGame(users_prop);
}

void Server::gameStarted()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    Connection *connection_sender = (Connection*)sender_object;

    for (int i = 0; i < _games.size(); ++i)
    {
        if(_games.at(i)._creator->userProperties() == connection_sender->userProperties())
        {
            _games[i]._started = true;
            i = _games.size();
        }
    }

    foreach(TcpConnection *connection, _tcp_connections)
        if(((Connection*)connection)->location() == TCP_LOCATION_IN_CHAT)
            ((Connection*)connection)->sendGameStarted(connection_sender->userProperties());

}
