#ifndef GAME_H
#define GAME_H

#include "team.h"

class Game
{
    public:

        static Game*            instance();

        ~Game();

        void                    init();

        void                    addTeam(Team *team);

        Team*                   getTeam1();

        Team*                   getTeam2();

        Player*                 getPlayerByNick(QString nick);

        QList<Player*>          getAllPlayers();

        QList<ITank*>           getAllTanks();

        void                    start();

        void                    pause();

        void                    resume();

        bool                    isPaused();

        qint64                  getCurrentTime();

        qint64                  serverLatency() const;

        void                    setServerLatency(const qint64 &server_latency);

    private:

        Game();

        static Game*            _instance;

        Team*                   _team_1;

        Team*                   _team_2;

        QElapsedTimer           _elapsed_timer;

        qint64                  _server_latency;

        bool                    _is_paused;
};

#endif // GAME_H
