#include "game.h"

Game *Game::_instance = NULL;

Game::Game()
{
    _team_1 = NULL;
    _team_2 = NULL;
}

Game *Game::instance()
{
    if(_instance == NULL)
        _instance = new Game();
    return _instance;
}

Game::~Game()
{
    if(_team_1 != NULL)
        delete _team_1;
    if(_team_2 != NULL)
        delete _team_2;
}

void Game::init()
{
    if(_team_1 != NULL)
    {
        delete _team_1;
        _team_1 = NULL;
    }
    if(_team_2 != NULL)
    {
        delete _team_2;
        _team_2 = NULL;
    }

    _server_latency = 0;
    _is_paused = false;
}

void Game::addTeam(Team *team)
{
    if(_team_1 == NULL)
        _team_1 = team;
    else if(_team_2 == NULL)
        _team_2 = team;
}

Team *Game::getTeam1()
{
    return _team_1;
}

Team *Game::getTeam2()
{
    return _team_2;
}

Player* Game::getPlayerByNick(QString nick)
{
    Player* player = _team_1->getPlayerByNick(nick);
    if(player == NULL)
        player = _team_2->getPlayerByNick(nick);
    return player;
}

QList<Player *> Game::getAllPlayers()
{
    QList<Player*> players = _team_1->getPlayers();
    players.append(_team_2->getPlayers());
    return players;
}

QList<ITank *> Game::getAllTanks()
{
    QList<ITank*> tanks = _team_1->getTanks();
    tanks.append(_team_2->getTanks());
    return tanks;
}

void Game::start()
{
    _elapsed_timer.start();
}

void Game::pause()
{
    if(!_is_paused)
        _is_paused = true;
}

void Game::resume()
{
    if(_is_paused)
        _is_paused = false;
}

bool Game::isPaused()
{
    return _is_paused;
}

qint64 Game::getCurrentTime()
{
    return _elapsed_timer.elapsed() + _server_latency;
}

qint64 Game::serverLatency() const
{
    return _server_latency;
}

void Game::setServerLatency(const qint64 &server_latency)
{
    _server_latency = server_latency;
}
