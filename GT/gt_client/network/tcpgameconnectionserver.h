#ifndef TCPGAMECONNECTIONSERVER_H
#define TCPGAMECONNECTIONSERVER_H

#include <QDateTime>

#include "tcpconnectionserver.h"
#include "gameglobals.h"

class TcpGameConnectionServer : public TcpConnectionServer
{
        Q_OBJECT

    public:

        explicit            TcpGameConnectionServer(bool use_ssl, int descriptor_socket, QObject *parent = 0);

        int                 index() const;

        void                sendRequestData();

        void                sendJoinedSuccessful(int index_of_terrain, QList<PlayerData> players_data, QList<int> random_numbers);

        void                sendNewPlayerJoined(PlayerData player_data);

        void                sendPlayerLeaveGame(PlayerData player_data, int new_index);

        void                sendGameFull();

        void                sendGameReadyToStart();

        void                sendGameStarted();

        void                sendPing();

        void                sendLatency(QMap<int, int> players_latency);

        void                sendMessage(QString message);

        void                sendPlayerReady(int index, bool value);

        void                sendChangeTeam(int index, int team);

        void                sendChangeTank(int index, int index_of_tank);

        void                sendPrepareGame();

        void                sendGameReady();

        void                sendStartSync();

        void                sendStartGameIn5Sec();

        void                sendActions(Actions word);

        void                sendGameOver();

    signals:

        void                notifyDataResponce(QString nick, QPixmap avatar, int rank);

        void                notifyPlayerLeaveGame(QString nick);

        void                notifyPong(int index, int latency);

        void                notifyMessage(QString message);

        void                notifyChangeTeam(int index);

        void                notifyChangeTank(int index, int index_of_tank);

        void                notifyPlayerReady(int index, bool value);

        void                notifyPrepareGame();
        void                notifyGameReady();

        void                notifyEndSync();

        void                notifyPlayerAction(PlayerAction player_action);

        void                notifyGameOver();

    protected:

        void                parseData(QDataStream &stream_input);

    private slots:

    private:

        void                parseDataResponce(QDataStream &stream_input);

        void                parsePlayerLeaveGame(QDataStream &stream_input);

        void                parsePong();

        void                parseMessage(QDataStream &stream_input);

        void                parseChangeTank(QDataStream &stream_input);

        void                parsePlayerReady(QDataStream &stream_input);

        void                parseStartSync();

        void                parseEndSync();

        void                parsePlayerAction(QDataStream &stream_input);

        void                parseGameMessage(QDataStream &stream_input);

        //------------------_att...

        int                 _index;

        quint64             _time_stamp_ping;
};

#endif // TCPGAMECONNECTIONSERVER_H
