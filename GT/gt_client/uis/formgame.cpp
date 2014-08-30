#include "formgame.h"
#include "ui_formgame.h"
#include "forms.h"
#include "dialog.h"

FormGame::FormGame(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormGame)
{
    _ui->setupUi(this);

    _logs = new QList<FadeLabelEffect*>();

    _ui->_l_foreground_skill_1->installEventFilter(this);
    _ui->_l_foreground_skill_2->installEventFilter(this);
    _ui->_l_foreground_skill_3->installEventFilter(this);
    _ui->_l_foreground_2->installEventFilter(this);

    _text_style_template = "<html><head><style type=\"text/css\">"
            "body {"
            "background-color: #00000000;"
            "}"
            "span{"
            "color: " + QColor(Qt::white).name(QColor::HexArgb) + ";"
            "}"
            ".black{"
            "color: " + QColor(Qt::black).name(QColor::HexArgb) + ";"
            "}"
            ".y{"
            "color: " + COLOR_YELOW + ";"
            "}"
            ".yl{"
            "color: " + COLOR_YELOW_LIGHT + ";"
            "}"
            ".g{"
            "color: " + COLOR_GREEN + ";"
            "}"
            ".gl{"
            "color: " + COLOR_GREEN_LEMON + ";"
            "}"
            ".gll{"
            "color: " + COLOR_GREEN_LEMON_LIGHT + ";"
            "}"
            ".r{"
            "color: " + COLOR_RED + ";"
            "}"
            ".rl{"
            "color: " + COLOR_RED_LIGHT + ";"
            "}"
            ".rd{"
            "color: " + COLOR_RED_DARK + ";"
            "}"
            ".p{"
            "color: " + COLOR_PURPLE + ";"
            "}"
            ".b{"
            "color: " + COLOR_BLUE + ";"
            "}"
            ".bl{"
            "color: " + COLOR_BLUE_LIGHT + ";"
            "}"
            ".o{"
            "color: " + COLOR_ORANGE + ";"
            "}"
            ".od{"
            "color: " + COLOR_ORANGE_DARK + ";"
            "}"
            ".w{"
            "color: " + COLOR_WHITE + ";"
            "}"
            "</style></head><body> %1 </body></html>";

    _skills_cooldown = getListOfPixmapFromStripImage(":/icons/skills_cooldown.png",72);

    _short_cut_time_control = new QShortcut(this);
    _short_cut_time_control->setKey(Qt::Key_F10);

    _short_cut_message = new QShortcut(this);
    _short_cut_message->setKey(Qt::Key_Return);

    _short_cut_message_for_all = new QShortcut(this);
    _short_cut_message_for_all->setKey(Qt::CTRL + Qt::Key_Return);

    _short_cut_scape = new QShortcut(this);
    _short_cut_scape->setKey(Qt::Key_Escape);

    connect(_short_cut_time_control, &QShortcut::activated, this, &FormGame::timeControl);
    connect(_short_cut_message, &QShortcut::activated, this, &FormGame::showChatMessage);
    connect(_short_cut_message_for_all, &QShortcut::activated, this, &FormGame::showChatMessageForAll);
    connect(_short_cut_scape, &QShortcut::activated, this, &FormGame::scapeShorcut);

    _timer = new QTimer(this);
    _timer->setTimerType(Qt::PreciseTimer);

    _tank = NULL;
    _game = NULL;
    _log_file = NULL;

    _ui->_l_fps->hide();

    //views game
    _scene = new QGraphicsScene(this);
    _scene->setBackgroundBrush(QColor(0,0,0));
    _ui->_gv_game->setScene(_scene);

    _mini_map_scene = new QGraphicsScene(this);
    _mini_map_scene->setBackgroundBrush(QColor(0,0,0));
    _ui->_gv_mini_map->setScene(_mini_map_scene);

    _tcp_game_server = TcpGameServer::instance();

    _tcp_game_client = TcpGameConnectionClient::instance();
    connect(_tcp_game_client, &TcpGameConnectionClient::notifyEndSync, this, &FormGame::clientEndSync);
    connect(_tcp_game_client, &TcpGameConnectionClient::notifyStartGamein5Sec, this, &FormGame::startGameIn5Sec);
    connect(_tcp_game_client, &TcpGameConnectionClient::notifyActionsChange, this, &FormGame::actionsChange);
    connect(_tcp_game_client, &TcpGameConnectionClient::notifyGameOver, this, &FormGame::gameOver);
    connect(_tcp_game_client, &TcpGameConnectionClient::notifyPlayerLeaveGame, this, &FormGame::playerLeaveGame);

    connect(Terrain::instance(), &Terrain::notifyError, this, &FormGame::terrainError);
    connect(Terrain::instance(), &Terrain::notifyLoadSteps, this, &FormGame::terrainLoadStep);
    connect(Terrain::instance(), &Terrain::notifyLoadFinished, this, &FormGame::terrainLoadFinished);

    connect(_timer, &QTimer::timeout, this, &FormGame::advance);
}

FormGame::~FormGame()
{
    delete _ui;

    if(_log_file != NULL)
    {
        _log_file->close();
        delete _log_file;
    }

    while(_tcp_game_client->isRunning())
    {
        _tcp_game_client->stop();
        _tcp_game_client->wait();
    }
    delete _tcp_game_client;
    delete _tcp_game_server;
}

void FormGame::hide()
{
    QCursor cursor = QCursor(QPixmap(":/cursor/cursor.png"), 8, 2);
    setCursor(cursor);

    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyDisconnected, this, &FormGame::clientGameDisconnected);
    Form::hide();
}

void FormGame::show(QList<PlayerData> players_data, int index_of_terrain, QPixmap presentation)
{    
    QCursor cursor = QCursor(QPixmap(":/cursor/cursor_2.png"), 16, 16);
    setCursor(cursor);

    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyDisconnected, this, &FormGame::clientGameDisconnected);
    connect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyDisconnected, this, &FormGame::clientGameDisconnected);
    disable();

    _actions.clear();
    _index_of_terrain = index_of_terrain;
    _players_data = players_data;
    _match_players.clear();
    _players_information.clear();
    _results.clear();
    _ui->_le_message->hide();
    _ui->_w_sound_music->hide();

    _ui->_vs_music->setValue(Configuration::volumeMusic());
    _ui->_vs_sound->setValue(Configuration::volumeSound());

    //show players information
    for(int i = 1; i <= 6; ++i)
        findChild<QWidget*>(QString("_w_player_%1").arg(i))->hide();

    int greens = 1;
    int reds = 1;

    int player_index = 0;
    foreach(PlayerData player_data, _players_data)
    {
        if(player_data._index_of_tank == -1)
        {
            player_data._index_of_tank = RandomNumberMannager::getMaxRandomNumber(ResourceManager::instance()->tankList().size()-1);
            _players_data.replace(player_index, player_data);
        }
        //add player information
        PlayerInformation player_information;
        player_information._nick        = player_data._nick;
        player_information._avatar      = player_data._avatar;
        player_information._rank        = player_data._rank;
        player_information._team        = player_data._team;
        player_information._tank_name   = ResourceManager::instance()->tankList().at(player_data._index_of_tank);
        _players_information.append(player_information);
        //------------------------------------------------------------------------

        int index_of_player = (player_data._team - 1)*3 + (player_data._team == 1 ? greens++ : reds++);

        _match_players.insert(player_index++, index_of_player);
        QMap<PlayerData, KillsDeaths> kills_deaths;
        foreach (PlayerData player_data_2, _players_data)
        {
            if(player_data_2._team != player_data._team)
            {
                KillsDeaths k_d;
                k_d._kills = 0;
                k_d._deaths = 0;
                k_d._circle_of_power_destroyed = false;
                kills_deaths.insert(player_data_2, k_d);
            }
        }
        _results.insert(player_data, kills_deaths);

        findChild<QWidget*>(QString("_w_player_%1").arg(index_of_player))->show();
        findChild<QLabel*>(QString("_l_nick_%1").arg(index_of_player))->setText(player_data._nick);
        findChild<QLabel*>(QString("_l_avatar_%1").arg(index_of_player))->setPixmap(player_data._avatar);
        findChild<QLabel*>(QString("_l_rank_%1").arg(index_of_player))->setPixmap(QPixmap(QString(":/gt/ranks/rank_%1.png").arg(player_data._rank)));
        findChild<QLabel*>(QString("_l_kills_%1").arg(index_of_player))->setText("0");
        findChild<QLabel*>(QString("_l_deaths_%1").arg(index_of_player))->setText("0");
        findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player))->hide();
        findChild<QLabel*>(QString("_l_level_%1").arg(index_of_player))->setText("1");
    }

    Form::show();

    //configurando controles
    foreach (int control, Configuration::controls())
        _controls.insert(control, false);

    //mostrar presentación
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->show();
    form_init_game->setPixmap(presentation);

    //borrando información
    _ui->_l_information->clear();
    _ui->_l_point_skill_1->clear();
    _ui->_l_point_skill_2->clear();
    _ui->_l_point_skill_3->clear();

    //cargando el terreno
    Terrain *terrain = Terrain::instance();
    terrain->load(ResourceManager::instance()->terrainList().at(index_of_terrain));

    //iniciando la escena
    form_init_game->setText(QString("Preparando escenario"));
    qApp->processEvents();
    prepareSceneAndView();

    form_init_game->setText(QString("Preparando elementos del juego"));
    qApp->processEvents();
    createGame();

    form_init_game->setText(QString("Esperando por otros jugadores."));
    qApp->processEvents();
    _tcp_game_client->sendGameReady();

    if(Configuration::activateLogs() && _tcp_game_client->isCreator())
    {
        if(_log_file != NULL)
            delete _log_file;

        _log_file = new QFile("game.log");
        if(_log_file->open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QDataStream stream(_log_file);
            stream << ResourceManager::instance()->terrainsInfo().at(_index_of_terrain) << RandomNumberMannager::_shema << _players_information << false;
            _log_file->close();
        }
    }
}

void FormGame::clientGameDisconnected()
{
    if(_tcp_game_client->isCreator())
        ConnectionTcp::instance()->sendCloseGame();
    else
        ConnectionTcp::instance()->sendLeaveGame();

    hide();
    Forms::instance()->_form_init_game->hide();
    Forms::instance()->_form_chat->show();
}

void FormGame::keyPressEvent(QKeyEvent *event)
{
    if(_ui->_le_message->isVisible())
    {
        QWidget::keyPressEvent(event);
        return;
    }
    if(event->isAutoRepeat())
    {
        event->ignore();
        return;
    }
    processKeyEvent(event->key(),true);
}

void FormGame::keyReleaseEvent(QKeyEvent *event)
{
    if(_ui->_le_message->isVisible())
    {
        QWidget::keyReleaseEvent(event);
        return;
    }
    if(event->isAutoRepeat())
    {
        event->ignore();
        return;
    }
    processKeyEvent(event->key(),false);
}

void FormGame::mousePressEvent(QMouseEvent *event)
{
    if(_ui->_le_message->isVisible())
    {
        QWidget::mousePressEvent(event);
        return;
    }
    processKeyEvent(event->button(),true);
}

void FormGame::mouseReleaseEvent(QMouseEvent *event)
{
    if(_ui->_le_message->isVisible())
    {
        QWidget::mouseReleaseEvent(event);
        return;
    }
    processKeyEvent(event->button(),false);
}

void FormGame::processKeyEvent(int key, bool value)
{
    if(_controls.contains(key))
        _controls.insert(key, value);
}

void FormGame::terrainError(QString error)
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText(error);
}

void FormGame::terrainLoadStep(QString step)
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText(step);
}

void FormGame::terrainLoadFinished()
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText("Terreno cargado");
}

void FormGame::clientEndSync(qint64 server_latency)
{
    Game::instance()->setServerLatency(server_latency);
}

void FormGame::startGameIn5Sec()
{
    _seconds_to_start_game = 6000 - Game::instance()->serverLatency();
    QTimer::singleShot(_seconds_to_start_game, Qt::PreciseTimer,this,SLOT(startGame()));
    QTimer::singleShot(Game::instance()->serverLatency(), this, SLOT(counterSec()));
}

void FormGame::counterSec()
{
    _seconds_to_start_game -= 1000;
    dynamic_cast<FormInitGame*>(Forms::instance()->_form_init_game)->setText(QString("El juego comienza en %1 segundos.").arg(_seconds_to_start_game/1000));

    if(_seconds_to_start_game > 0)
        _seconds_to_start_game >= 1000 ? QTimer::singleShot(1000, Qt::PreciseTimer, this, SLOT(counterSec())) : QTimer::singleShot(_seconds_to_start_game, Qt::PreciseTimer, this, SLOT(counterSec()));
}

void FormGame::updateCurrentLive(int value)
{
    updateIntegerLabel(_ui->_l_current_live, value, 100, COLOR_GREEN, COLOR_RED, false);
}

void FormGame::updateMaxLive(int value)
{
    updateIntegerLabel(_ui->_l_max_live, value, 100, COLOR_GREEN, COLOR_RED, false);
}

void FormGame::updateCurrentMana(int value)
{
    updateIntegerLabel(_ui->_l_current_mana, value, 100, COLOR_BLUE_LIGHT, COLOR_PURPLE, false);

    if(_tank->getSkill(1)->isActive() && !_tank->getSkill(1)->inCooldown())
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->getManaCost() <= value ? QPixmap() : _skill_locked);

    if(_tank->getSkill(2)->isActive() && !_tank->getSkill(2)->inCooldown())
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->getManaCost() <= value ? QPixmap() : _skill_locked);

    if(_tank->getSkill(3)->isActive() && !_tank->getSkill(3)->inCooldown())
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->getManaCost() <= value ? QPixmap() : _skill_locked);
}

void FormGame::updateMaxMana(int value)
{
    updateIntegerLabel(_ui->_l_max_mana, value, 100, COLOR_BLUE_LIGHT, COLOR_PURPLE, false);
}

void FormGame::updateEffect()
{
    updateIntegerLabel(_ui->_l_bonus_live_regeneration, _tank->getBonusLiveRegeneration(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_mana_regeneration, _tank->getBonusManaRegeneration(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_armor, _tank->getBonusArmor(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_speed, _tank->getBonusSpeed(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
}

void FormGame::updateTankView()
{
    _ui->_l_level->setText(QString::number(_tank->getLevel()));

    _ui->_l_current_live->setText(QString::number(_tank->currentLive()));
    _ui->_l_max_live->setText(QString::number(_tank->maxLive()));

    _ui->_l_current_mana->setText(QString::number(_tank->currentMana()));
    _ui->_l_max_mana->setText(QString::number(_tank->maxMana()));

    _ui->_l_level->setText(QString::number(_tank->getLevel()));

    _ui->_pb_experience->setMaximum(_tank->getNextLevelExperience());
    _ui->_pb_experience->setValue(_tank->getCurrentExperience());

    _ui->_pb_experience->setValue(0);

    updateIntegerLabel(_ui->_l_base_speed, _tank->baseSpeed());
    updateIntegerLabel(_ui->_l_bonus_speed, _tank->getBonusSpeed(), true, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);

    updateIntegerLabel(_ui->_l_base_armor, _tank->baseArmor());
    updateIntegerLabel(_ui->_l_bonus_armor, _tank->getBonusArmor(), true, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);

    updateIntegerLabel(_ui->_l_base_live_regeneration, _tank->getBaseLiveRegeneration());
    updateIntegerLabel(_ui->_l_bonus_live_regeneration, _tank->getBonusLiveRegeneration(), true, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);

    updateIntegerLabel(_ui->_l_base_mana_regeneration, _tank->getBaseManaRegeneration());
    updateIntegerLabel(_ui->_l_bonus_mana_regeneration, _tank->getBonusManaRegeneration(), true, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);

    updatePointsForUpdateSkills();
}

void FormGame::updateExperience(int value, int max)
{
    _ui->_pb_experience->setMaximum(max);
    _ui->_pb_experience->setValue(value);
    _ui->_l_experience->setText(QString("%1 / %2").arg(value).arg(max));
    _ui->_pb_experience->repaint();
}

void FormGame::updateSkill_1Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_1->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(), percent)));

    if(_tank->getSkill(1)->isActive() && !_tank->getSkill(1)->inCooldown())
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormGame::updateSkill_2Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_2->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(),percent)));

    if(_tank->getSkill(2)->isActive() && !_tank->getSkill(2)->inCooldown())
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormGame::updateSkill_3Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_3->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(),percent)));

    if(_tank->getSkill(3)->isActive() && !_tank->getSkill(3)->inCooldown())
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormGame::updateTankDeaths(ITank *killer)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    if(index > -1)
    {
        int index_of_player = _match_players.value(index);
        QLabel* label = findChild<QLabel*>(QString("_l_deaths_%1").arg(index_of_player));
        label->setText(QString::number(label->text().toInt() + 1));
    }

    int killer_index = _tanks.indexOf(killer);

    if(killer_index > -1)
    {
        int index_of_player = _match_players.value(killer_index);
        QLabel* label = findChild<QLabel*>(QString("_l_kills_%1").arg(index_of_player));
        label->setText(QString::number(label->text().toInt() + 1));
    }
    if(tank == _tank)
    {
        updateTankView();
    }

    PlayerData pd1 = _players_data.at(_tanks.indexOf(killer));
    PlayerData pd2 = _players_data.at(_tanks.indexOf(tank));

    if(killer->isEnemy() != tank->isEnemy())
    {
        KillsDeaths kills = _results.value(pd1).value(pd2);
        kills._kills += 1;
        QMap<PlayerData , KillsDeaths> kills_value;
        kills_value.insert(pd2, kills);
        _results.insert(pd1, kills_value);

        KillsDeaths deaths = _results.value(pd2).value(pd1);
        deaths._deaths += 1;
        QMap<PlayerData , KillsDeaths> deaths_value;
        deaths_value.insert(pd1, deaths);
        _results.insert(pd2, deaths_value);
    }

    QString pd1_team = pd1._team == 1?"g":"r";
    QString pd2_team = pd2._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha aniquilado a </span><span class='%3'>%4</span>");
    addLog(_text_style_template.arg(log.arg(pd1_team).arg(pd1._nick).arg(pd2_team).arg(pd2._nick)));
}

void FormGame::updateTankLevel(int value)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    if(index > -1)
    {
        int index_of_player = _match_players.value(index);
        QLabel* label = findChild<QLabel*>(QString("_l_level_%1").arg(index_of_player));
        label->setText(QString::number(value));
    }
}

void FormGame::updateTankTimeToReborn(int value)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    if(index > -1)
    {
        int index_of_player = _match_players.value(index);
        QLabel* label = findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player));
        label->show();
        label->setText(QString::number(value));
    }
}

void FormGame::updateTankReborn()
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    if(index > -1)
    {
        int index_of_player = _match_players.value(index);
        QLabel* label = findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player));
        label->clear();
        label->hide();
    }
    if(tank == _tank)
    {
        updateTankView();
    }
}

void FormGame::circleOfPowerDesactive(ITank* killer)
{
    CircleOfPower* circle_of_power = (CircleOfPower*)sender();
    circle_of_power->team() == 1 ? ++_green_circles_destroyers:++_red_circles_destroyers;

    PlayerData pd1 = _players_data.at(_tanks.indexOf(killer));
    PlayerData pd2 = _players_data.at(_tanks.indexOf(circle_of_power->tank()));

    if(killer->isEnemy() != circle_of_power->tank()->isEnemy())
    {
        KillsDeaths kills = _results.value(pd1).value(pd2);
        kills._circle_of_power_destroyed = true;
        QMap<PlayerData , KillsDeaths> value;
        value.insert(pd2, kills);
        _results.insert(pd1, value);
    }

    if(_green_circles_destroyers == _game->getTeam1()->getTanks().size())
    {
        showResults(2);
    }
    else if (_red_circles_destroyers == _game->getTeam2()->getTanks().size())
    {
        showResults(1);
    }
    else
    {
        QString pd1_team = pd1._team == 1?"g":"r";
        QString pd2_team = pd2._team == 1?"g":"r";
        QString log("<span class='%1'>%2</span><span> ha destruido el centro de energía de </span><span class='%3'>%4</span>");
        addLog(_text_style_template.arg(log.arg(pd1_team).arg(pd1._nick).arg(pd2_team).arg(pd2._nick)));
    }
}

void FormGame::gameOver()
{
    if(_tcp_game_client->isCreator())
        _tcp_game_server->close();
}

void FormGame::showResults(int team_win)
{
    disconnect(TcpGameConnectionClient::instance(), &TcpGameConnectionClient::notifyDisconnected, this, &FormGame::clientGameDisconnected);
    _timer->stop();

    if(_tcp_game_client->isCreator())
        ConnectionTcp::instance()->sendGameOver(team_win, _results);

    _tcp_game_client->sendGameOver();

    hide();
    Forms* forms = Forms::instance();
    forms->_form_result->show(team_win, _players_information, _results, ResourceManager::instance()->terrainsInfo().at(_index_of_terrain), _actions);
}

void FormGame:: createGame()
{
    _green_circles_destroyers = 0;
    _red_circles_destroyers = 0;

    RandomNumberMannager::_index = 0;

    _tanks.clear();

    _timing = 0;
    _game =  Game::instance();
    _game->init();

    Team* team_1 = new Team(GREEN, "Green", _game);
    Team* team_2 = new Team(RED, "Red", _game);

    for(int i = 0; i < _players_data.size(); ++i)
    {
        PlayerData pd = _players_data.at(i);
        Player* player = new Player(pd._nick, pd._rank, pd._index_of_tank, _scene, _mini_map_scene, (pd._team == 1 ? team_1: team_2));
        player->getTank()->setObstructecAreas(Terrain::instance()->getObstructedAreas());
        player->getTank()->setCorner(Terrain::instance()->getWidthInPX(), Terrain::instance()->getHeightInPX());
        _tanks.append(player->getTank());

        connect(player->getTank(), &ITank::notifyDead, this, &FormGame::updateTankDeaths);
        connect(player->getTank(), &ITank::notifyLevelUp, this, &FormGame::updateTankLevel);
        connect(player->getTank(), &ITank::notifyTimeToReborn, this, &FormGame::updateTankTimeToReborn);
        connect(player->getTank(), &ITank::notifyReborn, this, &FormGame::updateTankReborn);
    }

    int index_of_this_player = TcpGameConnectionClient::instance()->index();
    if(_players_data.at(index_of_this_player)._team == 1)
        team_2->setLikeEnemy();
    else
        team_1->setLikeEnemy();

    foreach(IObject * object, Terrain::instance()->objects())
    {
        object->setTanks(_tanks);
        if(object->inherits("CircleOfPower"))
        {
            CircleOfPower* cp = dynamic_cast<CircleOfPower*>(object);
            Team* team = cp->team() == 1 ? team_1 : team_2;
            if(cp->player() <= team->getTanks().size())
                cp->setTank(team->getTanks().at(cp->player()-1));

            connect(cp, &CircleOfPower::notifyDesactive, this, &FormGame::circleOfPowerDesactive);
        }
        object->initObject();
    }

    _tank = _tanks.at(index_of_this_player);

    foreach (ITank* tank, _tanks) {
        tank->setPrincipalReference(_tank);
        tank->initTank();
    }

    _ui->_l_preview_tank->setPixmap(_tank->getPreview());

    _preview_skill_1 = _tank->getSkill(1)->getPreview();
    _preview_skill_2 = _tank->getSkill(2)->getPreview();
    _preview_skill_3 = _tank->getSkill(3)->getPreview();

    _no_active_skill_1 = getNoActiveSkill(_preview_skill_1);
    _no_active_skill_2 = getNoActiveSkill(_preview_skill_2);
    _no_active_skill_3 = getNoActiveSkill(_preview_skill_3);

    _skill_locked = QPixmap(":/icons/skill_locked.png");

    _ui->_l_tank_name->setText(_tank->getName());

    _ui->_l_preview_skill_1->setPixmap(QPixmap());
    _ui->_l_preview_skill_2->setPixmap(QPixmap());
    _ui->_l_preview_skill_3->setPixmap(QPixmap());

    _ui->_l_foreground_skill_1->setPixmap(QPixmap());
    _ui->_l_foreground_skill_2->setPixmap(QPixmap());
    _ui->_l_foreground_skill_3->setPixmap(QPixmap());

    _ui->_l_information->setText(_text_style_template.arg(QString("<span>%1</span>").arg(_tank->getDescription())));

    _ui->_l_preview_skill_1->setPixmap(_no_active_skill_1);
    _ui->_l_preview_skill_2->setPixmap(_no_active_skill_2);
    _ui->_l_preview_skill_3->setPixmap(_no_active_skill_3);

    updateTankView();

    connect(_tank, &ITank::notifyCurrentLive, this, &FormGame::updateCurrentLive);
    connect(_tank, &ITank::notifyCurrentMana, this, &FormGame::updateCurrentMana);
    connect(_tank, &ITank::notifyEffectChange, this, &FormGame::updateEffect);
    connect(_tank, SIGNAL(notifyLevelUp(int)), this, SLOT(updateTankView()));
    connect(_tank, &ITank::notifyExperience, this, &FormGame::updateExperience);
    connect(_tank, &ITank::notifySkillLevelUp, this, &FormGame::updateSkill);

    connect(_tank->getSkill(1),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormGame::updateSkill_1Cooldown);
    connect(_tank->getSkill(2),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormGame::updateSkill_2Cooldown);
    connect(_tank->getSkill(3),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormGame::updateSkill_3Cooldown);
}

void FormGame::prepareSceneAndView()
{
    Terrain* terrain = Terrain::instance();

    //prepara la escena
    _scene->clear();
    _scene->setSceneRect(0, 0, terrain->getWidthInPX(), terrain->getHeightInPX());

    QPixmap mini_map = terrain->miniMapView().scaled(_ui->_gv_mini_map->width(), _ui->_gv_mini_map->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    _mini_map_scene->clear();
    _mini_map_scene->setSceneRect(0,0, mini_map.width(), mini_map.height());

    //add background
    _mini_map_scene->addPixmap(mini_map);

    //adiciona los tiles a la escena
    foreach (QGraphicsPixmapItem* tile, terrain->map())
        _scene->addItem(tile);

    //adiciona los objetos cargados a la escena
    foreach (IObject *object, terrain->objects())
    {
        _scene->addItem(object);
        _mini_map_scene->addItem(object->getMiniMapView());
        object->setPos(object->pos());
    }
}

bool FormGame::eventFilter(QObject *object, QEvent *event)
{
    if(_tank == NULL)
        return false;

    if(object == _ui->_l_foreground_skill_1 && event->type() == QEvent::MouseMove)
    {
        _ui->_l_information->setText(_text_style_template.arg(_tank->getSkill(1)->getDetails()));
        return true;
    }
    else if(object == _ui->_l_foreground_skill_2 && event->type() == QEvent::MouseMove)
    {
        _ui->_l_information->setText(_text_style_template.arg(_tank->getSkill(2)->getDetails()));
        return true;
    }
    else if(object == _ui->_l_foreground_skill_3 && event->type() == QEvent::MouseMove)
    {
        _ui->_l_information->setText(_text_style_template.arg(_tank->getSkill(3)->getDetails()));
        return true;
    }
    else if(object == _ui->_l_foreground_2 && event->type() == QEvent::MouseMove)
    {
        _ui->_l_information->setText(_text_style_template.arg(QString("<span>%1</span>").arg(_tank->getDescription())));
        return true;
    }

    return false;
}

void FormGame::updateIntegerLabel(QLabel *label, int value, int index_negative_value, QColor positive, QColor negative, bool positive_simbol)
{
    QString text = QString::number(value);
    if(index_negative_value != INT_MIN)
    {
        if(value > index_negative_value)
        {
            if(positive_simbol)
                text = text.prepend('+');
            label->setStyleSheet(QString("color: rgb(%1, %2, %3);").arg(positive.red()).arg(positive.green()).arg(positive.blue()));
        }
        else
            label->setStyleSheet(QString("color: rgb(%1, %2, %3);").arg(negative.red()).arg(negative.green()).arg(negative.blue()));
    }
    label->setText(text);
}

void FormGame::updateSkill(int skill, int level)
{
    switch (skill)
    {
        case 1:
            _ui->_l_preview_skill_1->setPixmap(_preview_skill_1);
            _ui->_l_point_skill_1->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(level)));
        break;
        case 2:
            _ui->_l_preview_skill_2->setPixmap(_preview_skill_2);
            _ui->_l_point_skill_2->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(level)));
        break;
        case 3:
            _ui->_l_preview_skill_3->setPixmap(_preview_skill_3);
            _ui->_l_point_skill_3->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(level)));
        break;
    }

    updatePointsForUpdateSkills();
}

void FormGame::updatePointsForUpdateSkills()
{
    int updates_availables = _tank->getUpdatesAvailablesForSkills();

    _ui->_l_updates_availables->setText(QString::number(updates_availables));

    _ui->_l_plus_skill_1->setVisible(_tank->getSkill(1)->isReadyForUpdate());
    _ui->_l_plus_skill_2->setVisible(_tank->getSkill(2)->isReadyForUpdate());
    _ui->_l_plus_skill_3->setVisible(_tank->getSkill(3)->isReadyForUpdate());
}

void FormGame::addLog(QString log)
{
    if(_logs->size() >= 20)
        _logs->takeAt(0)->deleteLater();

    FadeLabelEffect* message = new FadeLabelEffect(log, _logs, this);
    _ui->_messages_layout->addWidget(message);
    message->show();
}

void FormGame::timeControl()
{
    _fs_pause_game = !_game->isPaused();
}

QPixmap FormGame::getNoActiveSkill(QPixmap base)
{
    QImage image = base.toImage();
    for(int i= 0; i < image.width(); ++i)
        for(int j= 0; j < image.height(); ++j)
        {
            QColor color = image.pixel(i, j);
            int decrement = 100;
            color.setRed(color.red() - decrement < 0 ? 0 : color.red() - decrement);
            color.setGreen(color.green() - decrement < 0 ? 0 : color.green() - decrement);
            color.setBlue(color.blue() - decrement < 0 ? 0 : color.blue() - decrement);
            image.setPixel(i, j, color.rgba());
        }
    return QPixmap::fromImage(image);
}

void FormGame::advance()
{
    qint64 current_time = _game->getCurrentTime();
    if(current_time >= _timing && _frame_counter%2 == 0)
    {
        advanceFrame();
        ++_frame_counter;
    }
    else if(current_time >= _timing && _send_data)
    {
        sendData();
        ++_frame_counter;
    }
}

void FormGame::advanceFrame()
{
    RandomNumberMannager::nextIndex();

    if(!_game->isPaused())
    {
        _scene->advance();
        _ui->_gv_game->centerOn(_tank);
    }

    _timing += TICK;
}

void FormGame::sendData()
{
    QList<int> keys = Configuration::controls();

    PlayerAction player_action;
    player_action._player_index = _tanks.indexOf(_tank);

    player_action._message = _fs_message;
    player_action._for_all = _fs_for_all;

    player_action._time_control_change = _fs_pause_game != _game->isPaused();
    if(player_action._time_control_change)
        player_action._pause_game = _fs_pause_game;

    //calculate turret rotaion
    QPoint tank_pos = _ui->_gv_game->mapToGlobal(_ui->_gv_game->mapFromScene(_tank->scenePos()));
    QLineF line_cursor(tank_pos, QCursor::pos());

    qreal cursor_angle = ((360 - ((int)line_cursor.angle()))+90)%360;
    player_action._turret_rotation = cursor_angle;

    player_action._data_control_changed = _controls != _last_controls;
    if(player_action._data_control_changed)
    {
        player_action._move_fordward = _controls.value(keys.at(0));
        player_action._move_backward = _controls.value(keys.at(1));
        player_action._rotate_left = _controls.value(keys.at(2));
        player_action._rotate_right = _controls.value(keys.at(3));

        player_action._execute_skill_1 = _controls.value(keys.at(4));
        player_action._execute_skill_2 = _controls.value(keys.at(5));
        player_action._execute_skill_3 = _controls.value(keys.at(6));
        player_action._update_skill_1 = _controls.value(keys.at(7));
        player_action._update_skill_2 = _controls.value(keys.at(8));
        player_action._update_skill_3 = _controls.value(keys.at(9));

        _last_controls = _controls;
    }

    _tcp_game_client->sendPlayerAction(player_action);
    _send_data = false;

    _fs_message.clear();
    _fs_pause_game = _game->isPaused();

    //clean keys for update skills
    _controls.insert(Configuration::controls().at(7), false);
    _controls.insert(Configuration::controls().at(8), false);
    _controls.insert(Configuration::controls().at(9), false);
}

void FormGame::actionsChange(Actions actions)
{
    if(Configuration::activateLogs() && _tcp_game_client->isCreator())
    {
        if(_log_file->open(QIODevice::WriteOnly|QIODevice::Append))
        {
            QDataStream stream(_log_file);
            stream << actions;
            _log_file->close();
        }
    }
    _actions.append(actions);

    foreach(PlayerAction player_action, actions._player_actions)
    {
        //message
        if(!player_action._message.isEmpty())
        {
            PlayerData pd = _players_data.at(player_action._player_index);

            if(pd._team == _players_data.at(_tcp_game_client->index())._team || player_action._for_all)
            {
                QString team = pd._team == 1?"g":"r";
                QString for_all = player_action._for_all?"bl":"w";
                QString log("<span class='%1'>%2: </span><span class='%3'>%4</span>");
                addLog(_text_style_template.arg(log.arg(team).arg(pd._nick).arg(for_all).arg(player_action._message)));
            }
        }

        //pause or resume game
        if(player_action._time_control_change)
            player_action._pause_game? pauseGame(player_action._player_index):resumeGame(player_action._player_index);

        //tank
        if(!_game->isPaused())
        {
            ITank* tank = _tanks.at(player_action._player_index);

            tank->setTurretRotation(player_action._turret_rotation);

            if(player_action._data_control_changed)
            {
                tank->setMoveForward(player_action._move_fordward);
                tank->setMoveBackward(player_action._move_backward);
                tank->setRotateLeft(player_action._rotate_left);
                tank->setRotateRight(player_action._rotate_right);

                tank->setHurledSkill(1, player_action._execute_skill_1);
                tank->setHurledSkill(2, player_action._execute_skill_2);
                tank->setHurledSkill(3, player_action._execute_skill_3);

                if(player_action._update_skill_1)
                    tank->updateSkill(1);
                if(player_action._update_skill_2)
                    tank->updateSkill(2);
                if(player_action._update_skill_3)
                    tank->updateSkill(3);
            }
        }
    }

    advanceFrame();

    _send_data = true;
}

void FormGame::pauseGame(int player_index)
{
    _game->pause();
    _fs_pause_game = _game->isPaused();

    _ui->_pb_time_control->setIcon(QPixmap(":/icons/pause.png"));

    PlayerData pd = _players_data.at(player_index);

    QString team = pd._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha pausado el juego</span>");
    addLog(_text_style_template.arg(log.arg(team).arg(pd._nick)));
}

void FormGame::resumeGame(int player_index)
{
    _game->resume();
    _fs_pause_game = _game->isPaused();

    _ui->_pb_time_control->setIcon(QPixmap(":/icons/play.png"));

    PlayerData pd = _players_data.at(player_index);

    QString team = pd._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha reanudado el juego</span>");
    addLog(_text_style_template.arg(log.arg(team).arg(pd._nick)));
    setFocus();
}

void FormGame::playerLeaveGame(PlayerData pd)
{
    QString team = pd._team == 1?"g":"r";
    QString log("<span class='%1'>%2:</span><span> ha abandonado el juego</span>");
    addLog(_text_style_template.arg(log.arg(team).arg(pd._nick)));
}

void FormGame::startGame()
{
    //iniciando el juego
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->hide();
    enable();
    setFocus();

    Game::instance()->start();
    _timing = 0;
    _frame_counter = 0;
    _send_data = true;
    _fs_pause_game = false;
    _fs_message.clear();
    _timer->start(1);
}

void FormGame::showChatMessage()
{
    if(_ui->_le_message->isVisible())
    {
        if(!_ui->_le_message->text().isEmpty())
        {
            _fs_message = _ui->_le_message->text();
            _fs_for_all = false;
        }
        _ui->_le_message->clear();
        _ui->_le_message->hide();
        setFocus();
    }
    else
    {
        _ui->_le_message->show();
        _ui->_le_message->setFocus();
    }
}

void FormGame::showChatMessageForAll()
{
    if(_ui->_le_message->isVisible())
    {
        if(!_ui->_le_message->text().isEmpty())
        {
            _fs_message = _ui->_le_message->text();
            _fs_for_all = true;
        }

        _ui->_le_message->clear();
        _ui->_le_message->hide();
        setFocus();
    }
}

void FormGame::scapeShorcut()
{
    _ui->_le_message->clear();
    _ui->_le_message->hide();
    setFocus();
}

void FormGame::on__pb_time_control_clicked()
{
    timeControl();
}

void FormGame::on__pb_chat_clicked()
{
    showChatMessage();
}

void FormGame::on__pb_sound_music_clicked(bool checked)
{
    _ui->_w_sound_music->setVisible(checked);
    setFocus();
}

void FormGame::on__pb_close_clicked()
{
    if(!_game->isPaused())
        _fs_pause_game = true;

    Dialog* dialog = new Dialog("¿Estás seguro que quieres abandonar la partida?");

    connect(dialog, &QDialog::accepted, this, &FormGame::closeGame);
    connect(dialog, &QDialog::rejected, this, &FormGame::cancelCloseGame);

    dialog->exec();

    delete dialog;
}

void FormGame::on__vs_sound_valueChanged(int value)
{
    Configuration::setVolumeSound(value);
    Configuration::save();
    setFocus();
}

void FormGame::on__vs_music_valueChanged(int value)
{
    Configuration::setVolumeMusic(value);
    Configuration::save();
    setFocus();
}

void FormGame::closeGame()
{
    _timer->stop();
    if(_tcp_game_client->isCreator())
        _tcp_game_server->close();
    else
        _tcp_game_client->sendPlayeLeaveGame(_players_data[_tcp_game_client->index()]._nick);
}

void FormGame::cancelCloseGame()
{
    if(_game->isPaused())
        _fs_pause_game = false;
}
