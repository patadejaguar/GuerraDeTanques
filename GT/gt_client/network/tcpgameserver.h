#ifndef TCPGAMESERVER_H
#define TCPGAMESERVER_H

#include <QBitArray>
#include <QTimer>

#include "tcpserver.h"
#include "tcputilities.h"
#include "tcpgameconnectionserver.h"

class TcpGameServer : public TcpServer
{
        Q_OBJECT

    public:

        static TcpGameServer*   instance();

        void                    startGame();

        QList<PlayerData>       playersData() const;

    signals:

        void                    notifyLog(QString log);

    public slots:

        bool                    open(int index_of_terrain, int counter_of_players);

        void                    close();

        void                    requestLatency();

    protected:

        void                    incomingConnection(int handle);

    private slots:

        void                    closeConnection();        

        void                    playerResponceData(QString nick, QPixmap avatar, int rank);

        void                    playerLeaveGame(QString nick);

        void                    playerPong(int index, int latency);

        void                    sendLatency();

        void                    playerMessage(QString message);

        void                    playerChangeTeam(int index);

        void                    playerChangeTank(int index, int index_of_tank);

        void                    playerReady(int index, bool value);

        void                    playerPrepareGame();

        void                    playerGameReady();

        void                    playerEndSync();

        void                    playerAction(PlayerAction player_action);

        void                    trySendActions();

        void                    gameOverCounter();

        void                    trySendGameOver();

    private:

        explicit TcpGameServer();

        static TcpGameServer*   _instance;

        int                     _index_of_terrain;

        int                     _max_players;

        int                     _counter_game_over;

        QList<int>              _random_numbers;

        QList<PlayerData>       _players;

        int                     _game_ready_counter;

        bool                    _game_started;

        int                     _syncs;

        int                     _players_commands_count;

        Actions                 _actions;

        QMap<int, int>          _players_ping;

};

#endif // TCPGAMESERVER_H
