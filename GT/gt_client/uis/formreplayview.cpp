#include "formreplayview.h"
#include "ui_formreplayview.h"
#include "forms.h"

FormReplayView::FormReplayView(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormReplayView)
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
    _skill_locked = QPixmap(":/icons/skill_locked.png");

    _short_cut_time_control = new QShortcut(this);
    _short_cut_time_control->setKey(Qt::Key_F10);

    _timer = new QTimer(this);
    _timer->setTimerType(Qt::PreciseTimer);

    _tank = NULL;
    _game = NULL;

    //views game
    _scene = new QGraphicsScene();
    _scene->setBackgroundBrush(QColor(0,0,0));
    _ui->_gv_game->setScene(_scene);

    _mini_map_scene = new QGraphicsScene();
    _mini_map_scene->setBackgroundBrush(QColor(0,0,0));
    _ui->_gv_mini_map->setScene(_mini_map_scene);

    connect(_short_cut_time_control, &QShortcut::activated, this, &FormReplayView::timeControl);

    connect(Terrain::instance(), &Terrain::notifyError, this, &FormReplayView::terrainError);
    connect(Terrain::instance(), &Terrain::notifyLoadSteps, this, &FormReplayView::terrainLoadStep);
    connect(Terrain::instance(), &Terrain::notifyLoadFinished, this, &FormReplayView::terrainLoadFinished);

    connect(_timer, &QTimer::timeout, this, &FormReplayView::advance);

    //    _ui->_gv_game->setViewport(new QGLWidget(QGLFormat(QGL::Rgba | QGL::SampleBuffers | QGL::DirectRendering | QGL::AlphaChannel), this));
    //    _ui->_gv_mini_map->setViewport(new QGLWidget(QGLFormat(QGL::Rgba | QGL::SampleBuffers | QGL::DirectRendering | QGL::AlphaChannel), this));
}

FormReplayView::~FormReplayView()
{
    delete _ui;
}

bool FormReplayView::eventFilter(QObject *object, QEvent *event)
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

void FormReplayView::show(QString replay_file)
{
    disable();

    QFile file(DIR_REPLAYS.absoluteFilePath(replay_file));
    if(!file.open(QIODevice::ReadOnly))
    {
        on__pb_close_clicked();
        return;
    }
    QDataStream stream(&file);

    TerrainInfo                 terrain_info;
    QList<int>                  random_numbers_shema;
    bool                        action_list;

    stream >> terrain_info >> random_numbers_shema >> _players_information >> action_list;
    if(action_list)
        stream >> _actions;
    else
        while(!file.atEnd())
        {
            Actions actions;
            stream >> actions;
            _actions.append(actions);
        }
    file.close();

    _total_of_actions = _actions.size();

    ResourceManager::instance()->updateLocalResources();
    RandomNumberMannager::setShema(random_numbers_shema);

    _match_players.clear();
    _ui->_pb_time_game->setValue(0);
    _ui->_w_sound_music->hide();
    _ui->_cb_players->clear();

    _ui->_vs_music->setValue(Configuration::volumeMusic());
    _ui->_vs_sound->setValue(Configuration::volumeSound());

    //show players information
    for(int i = 1; i <= 6; ++i)
        findChild<QWidget*>(QString("_w_player_%1").arg(i))->hide();

    int greens = 1;
    int reds = 1;

    int player_index = 0;

    foreach(PlayerInformation player_information, _players_information)
    {
        int index_of_player = (player_information._team - 1)*3 + (player_information._team == 1 ? greens++ : reds++);

        _match_players.insert(player_index++, index_of_player);

        findChild<QWidget*>(QString("_w_player_%1").arg(index_of_player))->show();
        findChild<QLabel*>(QString("_l_nick_%1").arg(index_of_player))->setText(player_information._nick);
        findChild<QLabel*>(QString("_l_avatar_%1").arg(index_of_player))->setPixmap(player_information._avatar);
        findChild<QLabel*>(QString("_l_rank_%1").arg(index_of_player))->setPixmap(QPixmap(QString(":/gt/ranks/rank_%1.png").arg(player_information._rank)));
        findChild<QLabel*>(QString("_l_kills_%1").arg(index_of_player))->setText("0");
        findChild<QLabel*>(QString("_l_deaths_%1").arg(index_of_player))->setText("0");
        findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player))->hide();
        findChild<QLabel*>(QString("_l_level_%1").arg(index_of_player))->setText("1");

        _ui->_cb_players->addItem(QIcon(player_information._avatar), player_information._nick);
    }

    Form::show();

    //mostrar presentación
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->show();
    form_init_game->setPixmap(terrain_info._image);

    //borrando información
    _ui->_l_information->clear();
    _ui->_l_point_skill_1->clear();
    _ui->_l_point_skill_2->clear();
    _ui->_l_point_skill_3->clear();

    //cargando el terreno
    Terrain *terrain = Terrain::instance();
    terrain->load(QString("%1.gmm").arg(terrain_info._name));

    //iniciando la escena
    form_init_game->setText(QString("Preparando escenario"));
    qApp->processEvents();
    prepareSceneAndView();

    form_init_game->setText(QString("Preparando elementos del juego"));
    qApp->processEvents();
    createGame(_players_information);

    form_init_game->hide();
    startGame();
}

void FormReplayView::prepareSceneAndView()
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

void FormReplayView::createGame(QList<PlayerInformation> players_information)
{
    _green_circles_destroyers = 0;
    _red_circles_destroyers = 0;

    _tanks.clear();

    _timing = 0;
    _game =  Game::instance();
    _game->init();

    _team_1 = new Team(GREEN, "Green", _game);
    _team_2 = new Team(RED, "Red", _game);

    for(int i = 0; i < players_information.size(); ++i)
    {
        PlayerInformation pi = players_information.at(i);
        Player* player = new Player(pi._nick, pi._rank, ResourceManager::instance()->tankList().indexOf(pi._tank_name), _scene, _mini_map_scene, (pi._team == 1 ? _team_1: _team_2));
        player->getTank()->setObstructecAreas(Terrain::instance()->getObstructedAreas());
        player->getTank()->setCorner(Terrain::instance()->getWidthInPX(), Terrain::instance()->getHeightInPX());
        _tanks.append(player->getTank());

        connect(player->getTank(), &ITank::notifyDead, this, &FormReplayView::updateTankDeaths);
        connect(player->getTank(), &ITank::notifyLevelUp, this, &FormReplayView::updateTankLevel);
        connect(player->getTank(), &ITank::notifyTimeToReborn, this, &FormReplayView::updateTankTimeToReborn);
        connect(player->getTank(), &ITank::notifyReborn, this, &FormReplayView::updateTankReborn);
    }

    foreach(IObject * object, Terrain::instance()->objects())
    {
        object->setTanks(_tanks);
        if(object->inherits("CircleOfPower"))
        {
            CircleOfPower* cp = dynamic_cast<CircleOfPower*>(object);
            Team* team = cp->team() == 1 ? _team_1 : _team_2;
            if(cp->player() <= team->getTanks().size())
                cp->setTank(team->getTanks().at(cp->player()-1));

            connect(cp, &CircleOfPower::notifyDesactive, this, &FormReplayView::circleOfPowerDesactive);
        }
        object->initObject();
    }

    foreach (ITank* tank, _tanks)
        tank->initTank();

    _tank = NULL;
    updateTankReference(0);
}

QPixmap FormReplayView::getNoActiveSkill(QPixmap base)
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

void FormReplayView::updateCurrentLive(int value)
{
    updateIntegerLabel(_ui->_l_current_live, value, 100, COLOR_GREEN, COLOR_RED, false);
}

void FormReplayView::updateMaxLive(int value)
{
    updateIntegerLabel(_ui->_l_max_live, value, 100, COLOR_GREEN, COLOR_RED, false);
}

void FormReplayView::updateCurrentMana(int value)
{
    updateIntegerLabel(_ui->_l_current_mana, value, 100, COLOR_BLUE_LIGHT, COLOR_PURPLE, false);

    if(_tank->getSkill(1)->isActive() && !_tank->getSkill(1)->inCooldown())
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->getManaCost() <= value ? QPixmap() : _skill_locked);

    if(_tank->getSkill(2)->isActive() && !_tank->getSkill(2)->inCooldown())
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->getManaCost() <= value ? QPixmap() : _skill_locked);

    if(_tank->getSkill(3)->isActive() && !_tank->getSkill(3)->inCooldown())
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->getManaCost() <= value ? QPixmap() : _skill_locked);
}

void FormReplayView::updateMaxMana(int value)
{
    updateIntegerLabel(_ui->_l_max_mana, value, 100, COLOR_BLUE_LIGHT, COLOR_PURPLE, false);
}

void FormReplayView::updateEffect()
{
    updateIntegerLabel(_ui->_l_bonus_live_regeneration, _tank->getBonusLiveRegeneration(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_mana_regeneration, _tank->getBonusManaRegeneration(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_armor, _tank->getBonusArmor(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
    updateIntegerLabel(_ui->_l_bonus_speed, _tank->getBonusSpeed(), 0, COLOR_GREEN_LEMON_LIGHT, COLOR_ORANGE_DARK);
}

void FormReplayView::updateExperience(int value, int max)
{
    _ui->_pb_experience->setMaximum(max);
    _ui->_pb_experience->setValue(value);
    _ui->_l_experience->setText(QString("%1 / %2").arg(value).arg(max));
    _ui->_pb_experience->repaint();
}

void FormReplayView::updateSkill_1Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_1->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(), percent)));

    if(_tank->getSkill(1)->isActive() && !_tank->getSkill(1)->inCooldown())
        _ui->_l_foreground_skill_1->setPixmap(_tank->getSkill(1)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormReplayView::updateSkill_2Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_2->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(),percent)));

    if(_tank->getSkill(2)->isActive() && !_tank->getSkill(2)->inCooldown())
        _ui->_l_foreground_skill_2->setPixmap(_tank->getSkill(2)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormReplayView::updateSkill_3Cooldown(int percent)
{
    if(percent <= 0)
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->isReadyToExecute() ? QPixmap() : _skill_locked);
    else
        _ui->_l_foreground_skill_3->setPixmap(_skills_cooldown.at(getPart(_skills_cooldown.size(),percent)));

    if(_tank->getSkill(3)->isActive() && !_tank->getSkill(3)->inCooldown())
        _ui->_l_foreground_skill_3->setPixmap(_tank->getSkill(3)->getManaCost() <= _tank->currentMana() ? QPixmap() : _skill_locked);
}

void FormReplayView::updateTankReference(int index_of_tank)
{
    if(_tank != NULL)
    {
        disconnect(_tank, &ITank::notifyCurrentLive, this, &FormReplayView::updateCurrentLive);
        disconnect(_tank, &ITank::notifyCurrentMana, this, &FormReplayView::updateCurrentMana);
        disconnect(_tank, &ITank::notifyEffectChange, this, &FormReplayView::updateEffect);
        disconnect(_tank, SIGNAL(notifyLevelUp(int)), this, SLOT(updateTankView()));
        disconnect(_tank, &ITank::notifyExperience, this, &FormReplayView::updateExperience);
        disconnect(_tank, &ITank::notifySkillLevelUp, this, &FormReplayView::updateSkill);

        disconnect(_tank->getSkill(1),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_1Cooldown);
        disconnect(_tank->getSkill(2),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_2Cooldown);
        disconnect(_tank->getSkill(3),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_3Cooldown);
    }

    if(_players_information.at(index_of_tank)._team == 1)
        _team_2->setLikeEnemy();
    else
        _team_1->setLikeEnemy();

    _tank = _tanks.at(index_of_tank);
    _ui->_gv_game->centerOn(_tank);

    foreach (ITank* tank, _tanks)
        tank->setPrincipalReference(_tank);

    _ui->_l_preview_tank->setPixmap(_tank->getPreview());

    _preview_skill_1 = _tank->getSkill(1)->getPreview();
    _preview_skill_2 = _tank->getSkill(2)->getPreview();
    _preview_skill_3 = _tank->getSkill(3)->getPreview();

    _no_active_skill_1 = getNoActiveSkill(_preview_skill_1);
    _no_active_skill_2 = getNoActiveSkill(_preview_skill_2);
    _no_active_skill_3 = getNoActiveSkill(_preview_skill_3);

    _ui->_l_tank_name->setText(_tank->getName());

    _ui->_l_foreground_skill_1->setPixmap(QPixmap());
    _ui->_l_foreground_skill_2->setPixmap(QPixmap());
    _ui->_l_foreground_skill_3->setPixmap(QPixmap());

    _ui->_l_information->setText(_text_style_template.arg(QString("<span>%1</span>").arg(_tank->getDescription())));

    _ui->_l_preview_skill_1->setPixmap(_tank->getSkill(1)->isActive() ? _preview_skill_1 : _no_active_skill_1);
    _ui->_l_point_skill_1->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(_tank->getSkill(1)->getLevel())));

    _ui->_l_preview_skill_2->setPixmap(_tank->getSkill(2)->isActive() ? _preview_skill_2 : _no_active_skill_2);
    _ui->_l_point_skill_2->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(_tank->getSkill(2)->getLevel())));

    _ui->_l_preview_skill_3->setPixmap(_tank->getSkill(3)->isActive() ? _preview_skill_3 : _no_active_skill_3);
    _ui->_l_point_skill_3->setPixmap(QPixmap(QString(":icons/skill_point_%1.png").arg(_tank->getSkill(3)->getLevel())));


    connect(_tank, &ITank::notifyCurrentLive, this, &FormReplayView::updateCurrentLive);
    connect(_tank, &ITank::notifyCurrentMana, this, &FormReplayView::updateCurrentMana);
    connect(_tank, &ITank::notifyEffectChange, this, &FormReplayView::updateEffect);
    connect(_tank, SIGNAL(notifyLevelUp(int)), this, SLOT(updateTankView()));
    connect(_tank, &ITank::notifyExperience, this, &FormReplayView::updateExperience);
    connect(_tank, &ITank::notifySkillLevelUp, this, &FormReplayView::updateSkill);

    connect(_tank->getSkill(1),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_1Cooldown);
    connect(_tank->getSkill(2),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_2Cooldown);
    connect(_tank->getSkill(3),&ISkillFactory::notifyCurrentPercentCooldown, this, &FormReplayView::updateSkill_3Cooldown);

    updateTankView();
}

void FormReplayView::updateTankView()
{
    _ui->_l_level->setText(QString::number(_tank->getLevel()));

    updateCurrentLive(_tank->currentLive());
    updateMaxLive(_tank->maxLive());
    //    _ui->_l_current_live->setText(QString::number(_tank->currentLive()));
    //    _ui->_l_max_live->setText(QString::number(_tank->maxLive()));

    updateCurrentMana(_tank->currentMana());
    updateMaxMana(_tank->maxMana());
    //    _ui->_l_current_mana->setText(QString::number(_tank->currentMana()));
    //    _ui->_l_max_mana->setText(QString::number(_tank->maxMana()));

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

void FormReplayView::updateIntegerLabel(QLabel *label, int value, int index_negative_value, QColor positive, QColor negative, bool positive_symbol)
{
    QString text = QString::number(value);
    if(index_negative_value != INT_MIN)
    {
        if(value > index_negative_value)
        {
            if(positive_symbol)
                text = text.prepend('+');
            label->setStyleSheet(QString("color: rgb(%1, %2, %3);").arg(positive.red()).arg(positive.green()).arg(positive.blue()));
        }
        else
            label->setStyleSheet(QString("color: rgb(%1, %2, %3);").arg(negative.red()).arg(negative.green()).arg(negative.blue()));
    }
    label->setText(text);
}

void FormReplayView::updateSkill(int skill, int level)
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

void FormReplayView::updatePointsForUpdateSkills()
{
    int updates_availables = _tank->getUpdatesAvailablesForSkills();

    _ui->_l_updates_availables->setText(QString::number(updates_availables));

    _ui->_l_plus_skill_1->setVisible(_tank->getSkill(1)->isReadyForUpdate());
    _ui->_l_plus_skill_2->setVisible(_tank->getSkill(2)->isReadyForUpdate());
    _ui->_l_plus_skill_3->setVisible(_tank->getSkill(3)->isReadyForUpdate());
}

void FormReplayView::updateTankDeaths(ITank *killer)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    int index_of_player_sender = _match_players.value(index);
    QLabel* label_1 = findChild<QLabel*>(QString("_l_deaths_%1").arg(index_of_player_sender));
    label_1->setText(QString::number(label_1->text().toInt() + 1));

    int killer_index = _tanks.indexOf(killer);

    int index_of_player_killer = _match_players.value(killer_index);
    QLabel* label_2 = findChild<QLabel*>(QString("_l_kills_%1").arg(index_of_player_killer));
    label_2->setText(QString::number(label_2->text().toInt() + 1));

    if(tank == _tank)
        updateTankView();

    PlayerInformation pd1 = _players_information.at(killer_index);
    PlayerInformation pd2 = _players_information.at(index);

    QString pd1_team = pd1._team == 1?"g":"r";
    QString pd2_team = pd2._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha aniquilado a </span><span class='%3'>%4</span>");
    addLog(_text_style_template.arg(log.arg(pd1_team).arg(pd1._nick).arg(pd2_team).arg(pd2._nick)));
}

void FormReplayView::updateTankLevel(int value)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    int index_of_player = _match_players.value(index);
    QLabel* label = findChild<QLabel*>(QString("_l_level_%1").arg(index_of_player));
    label->setText(QString::number(value));
}

void FormReplayView::updateTankTimeToReborn(int value)
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    int index_of_player = _match_players.value(index);
    QLabel* label = findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player));
    label->show();
    label->setText(QString::number(value));
}

void FormReplayView::updateTankReborn()
{
    ITank* tank = (ITank*)sender();
    int index = _tanks.indexOf(tank);

    int index_of_player = _match_players.value(index);
    QLabel* label = findChild<QLabel*>(QString("_l_time_to_reborn_%1").arg(index_of_player));
    label->clear();
    label->hide();

    if(tank == _tank)
        updateTankView();
}

void FormReplayView::circleOfPowerDesactive(ITank* killer)
{
    CircleOfPower* circle_of_power = (CircleOfPower*)sender();
    circle_of_power->team() == 1 ? ++_green_circles_destroyers:++_red_circles_destroyers;

    PlayerInformation pd1 = _players_information.at(_tanks.indexOf(killer));
    PlayerInformation pd2 = _players_information.at(_tanks.indexOf(circle_of_power->tank()));

//    if(_green_circles_destroyers == _game->getTeam1()->getTanks().size())
//    {
//        //showResults(2);
//    }
//    else if (_red_circles_destroyers == _game->getTeam2()->getTanks().size())
//    {
//        //showResults(1);
//    }
//    else
//    {
        QString pd1_team = pd1._team == 1?"g":"r";
        QString pd2_team = pd2._team == 1?"g":"r";
        QString log("<span class='%1'>%2</span><span> ha destruido el centro de energía de </span><span class='%3'>%4</span>");
        addLog(_text_style_template.arg(log.arg(pd1_team).arg(pd1._nick).arg(pd2_team).arg(pd2._nick)));
//    }
}

void FormReplayView::timeControl()
{
    if(_timer->isActive())
    {
        _ui->_pb_time_control->setIcon(QPixmap(":/icons/pause.png"));
        _timer->stop();
    }
    else
    {
        _ui->_pb_time_control->setIcon(QPixmap(":/icons/play.png"));
        startGame();
    }
}

void FormReplayView::terrainError(QString error)
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText(error);
}

void FormReplayView::terrainLoadStep(QString step)
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText(step);
}

void FormReplayView::terrainLoadFinished()
{
    FormInitGame* form_init_game = Forms::instance()->_form_init_game;
    form_init_game->setText("Terreno cargado");
}

void FormReplayView::advance()
{
    qint64 current_time = _game->getCurrentTime();
    if(current_time < _timing)
        return;

    if(_actions.isEmpty())
        return;

    if(_frame_counter%2 != 0)
    {
        Actions actions = _actions.takeAt(0);
        _ui->_pb_time_game->setValue(100-(double)_actions.size()/(double)_total_of_actions*100);
        foreach(PlayerAction player_action, actions._player_actions)
        {
            //message
            if(!player_action._message.isEmpty())
            {
                PlayerInformation pi = _players_information.at(player_action._player_index);
                QString team = pi._team == 1?"g":"r";
                QString for_all = player_action._for_all?"bl":"w";
                QString log("<span class='%1'>%2: </span><span class='%3'>%4</span>");
                addLog(_text_style_template.arg(log.arg(team).arg(pi._nick).arg(for_all).arg(player_action._message)));
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
    }
    advanceFrame();
    ++_frame_counter;
}

void FormReplayView::advanceFrame()
{
    RandomNumberMannager::nextIndex();

    if(!_game->isPaused())
    {
        _scene->advance();
        _ui->_gv_game->centerOn(_tank);
    }

    _timing += TICK;
}

void FormReplayView::addLog(QString log)
{
    if(_logs->size() >= 20)
        _logs->takeAt(0)->deleteLater();

    FadeLabelEffect* message = new FadeLabelEffect(log, _logs, this);
    _ui->_messages_layout->addWidget(message);
    message->show();
}

void FormReplayView::startGame()
{
    _timing = 0;
    _frame_counter = 0;
    Game::instance()->setServerLatency(0);
    Game::instance()->start();
    _timer->start(1);
}

void FormReplayView::pauseGame(int player_index)
{
    _game->pause();

    PlayerInformation pi = _players_information.at(player_index);

    QString team = pi._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha pausado el juego</span>");
    addLog(_text_style_template.arg(log.arg(team).arg(pi._nick)));
}

void FormReplayView::resumeGame(int player_index)
{
    _game->resume();

    PlayerInformation pi = _players_information.at(player_index);

    QString team = pi._team == 1?"g":"r";
    QString log("<span class='%1'>%2</span><span> ha reanudado el juego</span>");
    addLog(_text_style_template.arg(log.arg(team).arg(pi._nick)));
    setFocus();
}

void FormReplayView::on__pb_close_clicked()
{
    _timer->stop();
    hide();
    Forms::instance()->_form_replays_select->show();
}

void FormReplayView::on__pb_time_control_clicked()
{
    timeControl();
}

void FormReplayView::on__pb_sound_music_clicked(bool checked)
{
    _ui->_w_sound_music->setVisible(checked);
}

void FormReplayView::on__vs_sound_valueChanged(int value)
{
    Configuration::setVolumeSound(value);
}

void FormReplayView::on__vs_music_valueChanged(int value)
{
    Configuration::setVolumeMusic(value);
}

void FormReplayView::on__cb_players_activated(int index)
{
    updateTankReference(index);
}
