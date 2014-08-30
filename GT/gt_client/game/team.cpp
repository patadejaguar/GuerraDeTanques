#include "team.h"
#include "game.h"

Team::Team(ColorTeam color_team, QString name, Game *game)
{
    _color_team = color_team;
    _name = name;
    _game = game;
    _game->addTeam(this);
}

Team::~Team()
{
    qDeleteAll(_players);
    _players.clear();
}

Team *Team::getEnemy()
{
    return _game->getTeam1() == this ? _game->getTeam2() : _game->getTeam1();
}

Player* Team::getPlayerByNick(QString nick)
{
    foreach (Player *player, _players)
    {
        if(player->getNick() == nick)
        {
            return player;
        }
    }
    return NULL;
}

void Team::setLikeEnemy(bool enemy)
{
    foreach(Player* player, _players)
        player->setLikeEnemy(enemy);
}

QList<ITank *> Team::getTanks()
{
    QList<ITank*> tanks;
    foreach(Player* player, _players)
        tanks.append(player->getTank());
    return tanks;
}

