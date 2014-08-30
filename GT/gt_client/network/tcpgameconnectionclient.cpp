#include "tcpgameconnectionclient.h"
#include "connectiontcp.h"

TcpGameConnectionClient* TcpGameConnectionClient::_instance = NULL;

TcpGameConnectionClient *TcpGameConnectionClient::instance()
{
    if(_instance == NULL)
        _instance = new TcpGameConnectionClient();
    return _instance;
}

int TcpGameConnectionClient::index() const
{
    return _index;
}

bool TcpGameConnectionClient::isCreator() const
{
    return _creator;
}

void TcpGameConnectionClient::sendPlayeLeaveGame(QString nick)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_LEAVE_GAME << nick;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendPong()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PONG;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendMessage(QString message)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_MESSAGE << message;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendChangeTeam()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_CHANGE_TEAM;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendChangeTank(int index_of_tank)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_CHANGE_TANK << index_of_tank;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendReady(bool value)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_READY << value;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendPrepareGame()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PREPARE_GAME;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendGameReady()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_GAME_READY;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendPlayerAction(PlayerAction player_action)
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_PLAYER_ACTION << player_action;
    sendData(data_to_write);
}

void TcpGameConnectionClient::sendGameOver()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_GAME_OVER;
    sendData(data_to_write);
}

void TcpGameConnectionClient::parseData(QDataStream &stream_input)
{
    quint8 head;
    stream_input >> head;

    switch (head)
    {
        case TCP_COMMAND_REQUEST_DATA:
            parseRequestData();
        break;
        case TCP_COMMAND_PLAYER_JOINED_SUCCESSFUL:
            parseJoinedSuccessful(stream_input);
        break;
        case TCP_COMMAND_NEW_PLAYER_JOINED:
            parseNewPlayerJoined(stream_input);
        break;
        case TCP_COMMAND_PLAYER_LEAVE_GAME:
            parsePlayerLeaveGame(stream_input);
        break;
        case TCP_COMMAND_GAME_FULL:
            parseGameFull();
        break;
        case TCP_COMMAND_GAME_READY_TO_START:
            parseGameReadyToStart();
        break;
        case TCP_COMMAND_GAME_IS_STARTED:
            parseGameIsStarted();
        break;
        case TCP_COMMAND_PING:
            parsePing();
        break;
        case TCP_COMMAND_LATENCY:
            parsePlayersLatency(stream_input);
        break;
        case TCP_COMMAND_MESSAGE:
            parseMessage(stream_input);
        break;
        case TCP_COMMAND_CHANGE_TEAM:
            parseChangeTeam(stream_input);
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

        case TCP_COMMAND_START_SYNC:
            parseStartSync();
        break;
        case TCP_COMMAND_END_SYNC:
            parseEndSync();
        break;
        case TCP_COMMAND_START_GAME_IN_5_SEC:
            parseStartGameIn5Sec();
        break;
        case TCP_COMMAND_PLAYER_ACTION:
            parsePlayerAction(stream_input);
        break;
        case TCP_COMMAND_WORD_CHANGE:
            parseActionsChange(stream_input);
        break;

        case TCP_GAME_OVER:
            emit notifyGameOver();
        break;
    }

}

void TcpGameConnectionClient::parseRequestData()
{
    ConnectionTcp* connection = ConnectionTcp::instance();
    UserProperties user_properties = connection->userProperties();

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_DATA_RESPONCE << user_properties._nick << user_properties._avatar << user_properties.rank();
    sendData(data_to_write);
}

void TcpGameConnectionClient::parseJoinedSuccessful(QDataStream &stream_input)
{
    int index_of_terrain;
    QList<PlayerData> players_data;
    QList<int> random_numbers;
    stream_input >> index_of_terrain >> players_data >> random_numbers;
    _index = players_data.size() - 1;
    _creator = _index == 0;
    emit notifyJoinedSuccessful(index_of_terrain, players_data, random_numbers);
}

void TcpGameConnectionClient::parseNewPlayerJoined(QDataStream &stream_input)
{
    PlayerData player_data;
    stream_input >> player_data;
    emit notifyNewPlayerJoined(player_data);
}

void TcpGameConnectionClient::parsePlayerLeaveGame(QDataStream &stream_input)
{
    PlayerData player_data;
    stream_input >> player_data >> _index;
    emit notifyPlayerLeaveGame(player_data);
}

void TcpGameConnectionClient::parseGameFull()
{
    stop();
    emit notifyGameFull();
}

void TcpGameConnectionClient::parseGameReadyToStart()
{
    stop();
    emit notifyGameReadyToStart();
}

void TcpGameConnectionClient::parseGameIsStarted()
{
    stop();
    emit notifyGameIsStarted();
}

void TcpGameConnectionClient::parsePing()
{
    sendPong();
}

void TcpGameConnectionClient::parsePlayersLatency(QDataStream &stream_input)
{
    QMap<int, int> players_latency;
    stream_input >> players_latency;
    emit notifyPlayersLatency(players_latency);
}

void TcpGameConnectionClient::parseMessage(QDataStream &stream_input)
{
    QString message;
    stream_input >> message;
    emit notifyMessage(message);
}

void TcpGameConnectionClient::parseChangeTeam(QDataStream &stream_input)
{
    int index, team;
    stream_input >> index >> team;
    emit notifyChangeTeam(index, team);
}

void TcpGameConnectionClient::parseChangeTank(QDataStream &stream_input)
{
    int index, index_of_tank;
    stream_input >> index >> index_of_tank;
    emit notifyChangeTank(index, index_of_tank);
}

void TcpGameConnectionClient::parsePlayerReady(QDataStream &stream_input)
{
    int index;
    bool value;
    stream_input >> index >> value;
    emit notifyPlayerReady(index, value);
}

void TcpGameConnectionClient::parseStartSync()
{
    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_START_SYNC;
    sendData(data_to_write);

    _sync_time.start();
}

void TcpGameConnectionClient::parseEndSync()
{
    emit notifyEndSync(_sync_time.elapsed() / 2);

    QByteArray data_to_write;
    QDataStream stream(&data_to_write,QIODevice::WriteOnly);
    stream << TCP_COMMAND_END_SYNC;
    sendData(data_to_write);
}

void TcpGameConnectionClient::parseStartGameIn5Sec()
{
    emit notifyStartGamein5Sec();
}

void TcpGameConnectionClient::parsePlayerAction(QDataStream &stream_input)
{
    PlayerAction player_action;
    stream_input >> player_action;

    emit notifyPlayerAction(player_action);
}

void TcpGameConnectionClient::parseActionsChange(QDataStream &stream_input)
{
    Actions word;
    stream_input >> word;

    emit notifyActionsChange(word);
}

TcpGameConnectionClient::TcpGameConnectionClient()
    :TcpClient()
{
    qRegisterMetaType< QMap<int, int> >("QMap<int, int>");
}
