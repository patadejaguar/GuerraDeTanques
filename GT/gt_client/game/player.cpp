#include "player.h"
#include "team.h"
#include "game.h"
#include "resourcemanager.h"

Player::Player(QString nick, int rank, int index_of_tank, QGraphicsScene *scene, QGraphicsScene* mini_map_scene, Team * team):
    QObject()
{
    _nick = nick;
    _color_team = team->getColorTeam();
    _rank = rank;
    _scene = scene;
    _team = team;

    _tank = ResourceManager::instance()->tankFactories().at(index_of_tank)->createInstance(_nick, _rank, _color_team);

    scene->addItem(_tank);
    mini_map_scene->addItem(_tank->miniMapTank());
    _tank->createInfoBar();

    Team* enemy_team = _team->getEnemy();
    foreach(Player* player, enemy_team->getPlayers())
    {
        player->getTank()->addEnemy(_tank);
        _tank->addEnemy(player->getTank());
    }

    foreach(Player* player, _team->getPlayers())
    {
        player->getTank()->addAllied(_tank);
        _tank->addAllied(player->getTank());
    }

    _team->addPlayer(this);
}
