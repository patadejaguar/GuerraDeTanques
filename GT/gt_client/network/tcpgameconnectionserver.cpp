#include "tcpgameconnectionserver.h"

TcpGameConnectionServer::TcpGameConnectionServer(bool use_ssl, int descriptor_socket, QObject *parent) :
    TcpConnectionServer(use_ssl, descriptor_socket, parent)
{
    _index = -1;
}

void TcpGameConnectionServer::sendRequestData()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_REQUEST_DATA;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendJoinedSuccessful(int index_of_terrain, QList<PlayerData> players_data, QList<int> random_numbers)
{
    QByteArray data_to_write;
    _index = players_data.size() - 1;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_JOINED_SUCCESSFUL << index_of_terrain << players_data << random_numbers;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendNewPlayerJoined(PlayerData player_data)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_NEW_PLAYER_JOINED << player_data;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendPlayerLeaveGame(PlayerData player_data, int new_index)
{
    _index = new_index;
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_LEAVE_GAME << player_data << new_index;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendGameFull()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_GAME_FULL;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendGameReadyToStart()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_GAME_READY_TO_START;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendGameStarted()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_GAME_IS_STARTED;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendPing()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PING;
    _time_stamp_ping = QDateTime::currentMSecsSinceEpoch();
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendLatency(QMap<int, int> players_latency)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_LATENCY << players_latency;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendMessage(QString message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_MESSAGE << message;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendPlayerReady(int index, bool value)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_READY << index << value;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendChangeTeam(int index, int team)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_CHANGE_TEAM << index << team;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendChangeTank(int index, int index_of_tank)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_CHANGE_TANK << index << index_of_tank;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendPrepareGame()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PREPARE_GAME;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendGameReady()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_GAME_READY;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendStartSync()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_START_SYNC;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendStartGameIn5Sec()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_START_GAME_IN_5_SEC;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendActions(Actions word)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_WORD_CHANGE << word;
    sendData(data_to_write);
}

void TcpGameConnectionServer::sendGameOver()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_GAME_OVER;
    sendData(data_to_write);
}

void TcpGameConnectionServer::parseData(QDataStream &stream_input)
{
    quint8 head;
    stream_input>>head;

    switch (head)
    {        
        case TCP_COMMAND_DATA_RESPONCE:
            parseDataResponce(stream_input);
        break;
        case TCP_COMMAND_PLAYER_LEAVE_GAME:
            parsePlayerLeaveGame(stream_input);
        break;
        case TCP_COMMAND_PONG:
            parsePong();
        break;
        case TCP_COMMAND_MESSAGE:
            parseMessage(stream_input);
        break;
        case TCP_COMMAND_CHANGE_TEAM:
            emit notifyChangeTeam(_index);
        break;
        case TCP_COMMAND_CHANGE_TANK:
            parseChangeTank(stream_input);
        break;
        case TCP_COMMAND_PLAYER_READY:
            parsePlayerReady(stream_input);
        break;
        case TCP_COMMAND_PREPARE_GAME:
            emit notifyPrepareGame();
        break;
        case TCP_COMMAND_GAME_READY:
            emit notifyGameReady();
        break;
        case TCP_COMMAND_START_SYNC:
            parseStartSync();
        break;
        case TCP_COMMAND_END_SYNC:
            parseEndSync();
        break;
        case TCP_COMMAND_PLAYER_ACTION:
            parsePlayerAction(stream_input);
        break;

        case TCP_GAME_OVER:
            emit notifyGameOver();
        break;
    }
}

void TcpGameConnectionServer::parseDataResponce(QDataStream &stream_input)
{
    QString nick;
    QPixmap avatar;
    int rank;
    stream_input >> nick >> avatar >> rank;
    emit notifyDataResponce(nick, avatar, rank);
}

void TcpGameConnectionServer::parsePlayerLeaveGame(QDataStream &stream_input)
{
    QString nick;
    stream_input >> nick;
    emit notifyPlayerLeaveGame(nick);
}

void TcpGameConnectionServer::parsePong()
{
    emit notifyPong(_index, int(QDateTime::currentMSecsSinceEpoch() - _time_stamp_ping));
}

void TcpGameConnectionServer::parseMessage(QDataStream &stream_input)
{
    QString message;
    stream_input >> message;
    emit notifyMessage(message);
}

void TcpGameConnectionServer::parseChangeTank(QDataStream &stream_input)
{
    int index_of_tank;
    stream_input >> index_of_tank;
    emit notifyChangeTank(_index, index_of_tank);
}

void TcpGameConnectionServer::parsePlayerReady(QDataStream &stream_input)
{
    bool value;
    stream_input >> value;
    emit notifyPlayerReady(_index, value);
}

void TcpGameConnectionServer::parseStartSync()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_END_SYNC;
    sendData(data_to_write);
}

void TcpGameConnectionServer::parseEndSync()
{
    emit notifyEndSync();
}

void TcpGameConnectionServer::parsePlayerAction(QDataStream &stream_input)
{
    PlayerAction player_action;
    stream_input >> player_action;

    emit notifyPlayerAction(player_action);
}

int TcpGameConnectionServer::index() const
{
    return _index;
}
