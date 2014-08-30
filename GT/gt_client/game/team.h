#ifndef TEAM_H
#define TEAM_H

#include <QList>
#include "player.h"

class Game;

class Team
{
    public:

        Team(ColorTeam color_team, QString name, Game* game);

        ~Team();

        Team*                   getEnemy();

        inline void             addPlayer(Player* player){_players.append(player);}

        inline QList<Player*>   getPlayers(){return _players;}

        Player*                 getPlayerByNick(QString nick);

        inline Game*            getGame(){return _game;}

        inline ColorTeam        getColorTeam(){return _color_team;}

        void                    setLikeEnemy(bool enemy = true);

        QList<ITank*>           getTanks();

    private:

        QString                 _name;

        QList<Player*>          _players;

        Game*                   _game;

        ColorTeam               _color_team;
};

#endif // TEAM_H
