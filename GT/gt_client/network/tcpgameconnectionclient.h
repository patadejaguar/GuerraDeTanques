#ifndef TCPGAMECONNECTIONCLIENT_H
#define TCPGAMECONNECTIONCLIENT_H

#include <QElapsedTimer>

#include "tcpclient.h"
#include "gameglobals.h"

class TcpGameConnectionClient : public TcpClient
{
        Q_OBJECT

    public:

        static TcpGameConnectionClient*   instance();

        int                     index() const;

        bool                    isCreator() const;

        void                    sendPlayeLeaveGame(QString nick);

        void                    sendPong();

        void                    sendMessage(QString message);

        void                    sendChangeTank(int index_of_tank);

        void                    sendReady(bool value);

        void                    sendPrepareGame();

        void                    sendGameReady();

        void                    sendPlayerAction(PlayerAction player_action);

        void                    sendGameOver();

    public slots:

        void                    sendChangeTeam();

    signals:

        void                    notifyJoinedSuccessful(int index_of_terrain, QList<PlayerData> _players, QList<int> random_numbers);

        void                    notifyNewPlayerJoined(PlayerData _player);

        void                    notifyPlayerLeaveGame(PlayerData _player);

        void                    notifyDataReceived(QByteArray data);

        void                    notifyGameFull();

        void                    notifyGameReadyToStart();

        void                    notifyGameIsStarted();

        void                    notifyPlayersLatency(QMap<int, int> players_latency);

        void                    notifyMessage(QString message);

        void                    notifyChangeTeam(int index, int team);

        void                    notifyChangeTank(int index, int index_of_tank);

        void                    notifyPlayerReady(int index, bool value);

        void                    notifyPrepareGame();

        void                    notifyStartSync();

        void                    notifyEndSync(qint64 time_server);

        void                    notifyStartGamein5Sec();

        void                    notifyPlayerAction(PlayerAction player_action);

        void                    notifyActionsChange(Actions word);

        void                    notifyGameOver();

    protected:

        void                    parseData(QDataStream &stream_input);


    private slots:


    private:

        void                    parseRequestData();

        void                    parseJoinedSuccessful(QDataStream &stream_input);

        void                    parseNewPlayerJoined(QDataStream &stream_input);

        void                    parsePlayerLeaveGame(QDataStream &stream_input);

        void                    parseGameFull();

        void                    parseGameReadyToStart();

        void                    parseGameIsStarted();

        void                    parsePing();

        void                    parsePlayersLatency(QDataStream &stream_input);

        void                    parseMessage(QDataStream &stream_input);

        void                    parseChangeTeam(QDataStream &stream_input);

        void                    parseChangeTank(QDataStream &stream_input);

        void                    parsePlayerReady(QDataStream &stream_input);

        void                    parseStartSync();

        void                    parseEndSync();

        void                    parseStartGameIn5Sec();

        void                    parsePlayerAction(QDataStream &stream_input);

        void                    parseActionsChange(QDataStream &stream_input);

                                TcpGameConnectionClient();

        //----------------------_att...

        int                     _index;

        bool                    _creator;

        static TcpGameConnectionClient*   _instance;

        QElapsedTimer           _sync_time;
};

#endif // TCPGAMECONNECTIONCLIENT_H
