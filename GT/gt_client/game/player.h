#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QGraphicsScene>

#include "configuration.h"
#include "infobar.h"
#include "itank.h"
#include "iskill.h"

class Team;
class Game;

class Player: public QObject
{
        Q_OBJECT

    public:

        explicit            Player(QString nick, int rank, int index_of_tank, QGraphicsScene *scene, QGraphicsScene* mini_map_scene, Team * team);

        inline QString      getNick(){return _nick;}

        inline int          getPointer(){return _color_team;}

        inline int          getRank(){return _rank;}

        inline ITank*       getTank(){return _tank;}

        inline Team*        getTeam(){return _team;}

        inline int          getCurrentLive(){return _tank->currentLive();}

        inline int          getMaxLive(){return _tank->maxLive();}

        inline int          getCurrentMana(){return _tank->currentMana();}

        inline int          getMaxMana(){return _tank->maxMana();}

        inline void         setLikeEnemy(bool enemy = true){_tank->setIsEnemy(enemy);}

    public slots:

    private:

        QString             _nick;

        int                 _rank;

        ColorTeam           _color_team;

        Team*               _team;

        ITank*              _tank;

        QGraphicsScene*     _scene;

};

#endif // PLAYER_H
