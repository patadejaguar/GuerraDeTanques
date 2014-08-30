#include "tcpgameserver.h"

TcpGameServer* TcpGameServer::_instance = NULL;

TcpGameServer *TcpGameServer::instance()
{
    if(_instance == NULL)
        _instance = new TcpGameServer();
    return _instance;
}

void TcpGameServer::startGame()
{
    if(!_game_started)
    {
        _game_started = true;
        _syncs = 0;
        _players_commands_count = 0;

        foreach(TcpConnection* tcp_connection, _tcp_connections)
            ((TcpGameConnectionServer*)tcp_connection)->sendStartSync();
    }
}

QList<PlayerData> TcpGameServer::playersData() const
{
    return _players;
}

bool TcpGameServer::open(int index_of_terrain, int counter_of_players)
{
    _index_of_terrain = index_of_terrain;
    _max_players = counter_of_players;
    _game_ready_counter = _counter_game_over = 0;

    _game_started = false;
    _syncs = 0;
    _players_commands_count = 0;

    qsrand(QDateTime::currentMSecsSinceEpoch());
    _random_numbers.clear();
    for(int i = 0; i < 1000; ++i)
        _random_numbers.append(qrand());

    return QTcpServer::listen(QHostAddress::Any, TCP_GAME_SERVER_PORT);
}

void TcpGameServer::close()
{
    _players.clear();
    TcpServer::close();
}

void TcpGameServer::incomingConnection(int handle)
{
    TcpGameConnectionServer *connection = new TcpGameConnectionServer(_use_ssl, handle);

    connect(connection, &TcpGameConnectionServer::notifyDisconnected, this, &TcpGameServer::closeConnection);
    connect(connection, &TcpGameConnectionServer::notifyDataResponce, this, &TcpGameServer::playerResponceData);
    connect(connection, &TcpGameConnectionServer::notifyPlayerLeaveGame, this, &TcpGameServer::playerLeaveGame);
    connect(connection, &TcpGameConnectionServer::notifyPong, this, &TcpGameServer::playerPong);
    connect(connection, &TcpGameConnectionServer::notifyMessage, this, &TcpGameServer::playerMessage);
    connect(connection, &TcpGameConnectionServer::notifyChangeTeam, this, &TcpGameServer::playerChangeTeam);
    connect(connection, &TcpGameConnectionServer::notifyChangeTank, this, &TcpGameServer::playerChangeTank);
    connect(connection, &TcpGameConnectionServer::notifyPlayerReady, this, &TcpGameServer::playerReady);
    connect(connection, &TcpGameConnectionServer::notifyPrepareGame, this, &TcpGameServer::playerPrepareGame);
    connect(connection, &TcpGameConnectionServer::notifyGameReady, this, &TcpGameServer::playerGameReady);

    connect(connection, &TcpGameConnectionServer::notifyEndSync, this, &TcpGameServer::playerEndSync);
    connect(connection, &TcpGameConnectionServer::notifyPlayerAction, this, &TcpGameServer::playerAction);

    connect(connection, &TcpGameConnectionServer::notifyGameOver, this, &TcpGameServer::gameOverCounter);

    addConnection(connection);

    if(_game_started)
        connection->sendGameStarted();
    else if(_players.size() > _max_players)
        connection->sendGameFull();
    else if(!_players.isEmpty() && _players[0]._ready)
        connection->sendGameReadyToStart();
    else
        connection->sendRequestData();
}

void TcpGameServer::closeConnection()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;
    TcpGameConnectionServer *connection_sender = (TcpGameConnectionServer*)sender_object;

    int index = connection_sender->index();

    if(index == 0)
        close();
    else if(index != -1)
    {
        foreach(TcpConnection* tcp_connection, _tcp_connections)
        {
            TcpGameConnectionServer *gcs = (TcpGameConnectionServer*)tcp_connection;
            if(gcs != connection_sender)
            {
                gcs->sendPlayerLeaveGame(_players.at(index), (index < gcs->index() && !_game_started) ? gcs->index()-1 : gcs->index());
            }
        }

        _players.removeAt(index);
    }

    TcpServer::closeConnection(connection_sender);

    trySendActions();
    trySendGameOver();
}

void TcpGameServer::playerResponceData(QString nick, QPixmap avatar, int rank)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    TcpGameConnectionServer *connection_sender = (TcpGameConnectionServer*)sender_object;

    PlayerData player_data;
    player_data._nick = nick;
    player_data._avatar = avatar;
    player_data._rank = rank;
    player_data._team = _players.size() < _max_players / 2 ? 1 : 2;
    player_data._index_of_tank = -1;
    player_data._ready = false;
    _players.append(player_data);

    connection_sender->sendJoinedSuccessful(_index_of_terrain, _players, _random_numbers);

    foreach(TcpConnection* tcp_connection, _tcp_connections)
        if(((TcpGameConnectionServer*)tcp_connection) != connection_sender)
            ((TcpGameConnectionServer*)tcp_connection)->sendNewPlayerJoined(player_data);
}

void TcpGameServer::playerLeaveGame(QString nick)
{
    int index = -1;

    for(int i = 0; i < _players.size(); ++i)
    {
        if(_players[i]._nick == nick)
        {
            index = i;
            i = _players.size();
        }
    }

    if(index != -1)
    {
        foreach(TcpConnection* tcp_connection, _tcp_connections)
        {
            TcpGameConnectionServer *gcs = (TcpGameConnectionServer*)tcp_connection;
            if(gcs->index() != index)
            {
                gcs->sendPlayerLeaveGame(_players.at(index), (index < gcs->index() && !_game_started) ? gcs->index()-1 : gcs->index());
            }
        }

        _players.removeAt(index);

        TcpServer::closeConnection((TcpConnectionServer*)(_tcp_connections.at(index)));
    }

}

void TcpGameServer::requestLatency()
{
    _players_ping.clear();

    foreach(TcpConnection* tcp_connection, _tcp_connections)
    {
        TcpGameConnectionServer *gcs = (TcpGameConnectionServer*)tcp_connection;
        if(gcs->index() != -1)
            gcs->sendPing();
    }
}

void TcpGameServer::playerPong(int index, int latency)
{
    _players_ping.insert(index, latency);
    if(_players_ping.size() == _tcp_connections.size())
        sendLatency();
}

void TcpGameServer::sendLatency()
{
    foreach(TcpConnection* tcp_connection, _tcp_connections)
    {
        TcpGameConnectionServer *gcs = (TcpGameConnectionServer*)tcp_connection;
        gcs->sendLatency(_players_ping);
    }
    _players_ping.clear();
}

void TcpGameServer::playerMessage(QString message)
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;

    TcpGameConnectionServer *connection_sender = (TcpGameConnectionServer*)sender_object;

    QString nick = _players.at(connection_sender->index())._nick;

    foreach(TcpConnection* tcp_connection, _tcp_connections)
        ((TcpGameConnectionServer*)tcp_connection)->sendMessage(QString("%1: %2").arg(nick).arg(message));
}

void TcpGameServer::playerChangeTeam(int index)
{
    _players[index].changeTeam();

    foreach(TcpConnection* tcp_connection, _tcp_connections)
        ((TcpGameConnectionServer*)tcp_connection)->sendChangeTeam(index, _players[index]._team);
}

void TcpGameServer::playerChangeTank(int index, int index_of_tank)
{
    _players[index].changeTank(index_of_tank);

    foreach(TcpConnection* tcp_connection, _tcp_connections)
        ((TcpGameConnectionServer*)tcp_connection)->sendChangeTank(index, _players[index]._index_of_tank);

}

void TcpGameServer::playerReady(int index, bool value)
{
    _players[index].setReady(value);

    foreach(TcpConnection* tcp_connection, _tcp_connections)
        ((TcpGameConnectionServer*)tcp_connection)->sendPlayerReady(index, value);
}

void TcpGameServer::playerPrepareGame()
{
    foreach(TcpConnection* tcp_connection, _tcp_connections)
        ((TcpGameConnectionServer*)tcp_connection)->sendPrepareGame();
}

void TcpGameServer::playerGameReady()
{
    ++_game_ready_counter;
    if(_game_ready_counter == _players.size())
        startGame();
}

void TcpGameServer::playerEndSync()
{
    ++_syncs;

    if(_syncs == _tcp_connections.size())
    {
        foreach(TcpConnection* tcp_connection, _tcp_connections)
            ((TcpGameConnectionServer*)tcp_connection)->sendStartGameIn5Sec();
    }
}

void TcpGameServer::playerAction(PlayerAction player_action)
{
    _actions._player_actions.append(player_action);
    trySendActions();
}

void TcpGameServer::trySendActions()
{
    if(_actions._player_actions.size() >= _tcp_connections.size())
    {
        foreach(TcpConnection* tcp_connection, _tcp_connections)
            ((TcpGameConnectionServer*)tcp_connection)->sendActions(_actions);

        _actions._player_actions.clear();
    }
}

void TcpGameServer::gameOverCounter()
{
    ++_counter_game_over;
    trySendGameOver();
}

void TcpGameServer::trySendGameOver()
{
    if(_counter_game_over == _tcp_connections.size())
    {
        foreach(TcpConnection* tcp_connection, _tcp_connections)
            ((TcpGameConnectionServer*)tcp_connection)->sendGameOver();
        _counter_game_over = 0;
    }
}

TcpGameServer::TcpGameServer() :
    TcpServer()
{

}
