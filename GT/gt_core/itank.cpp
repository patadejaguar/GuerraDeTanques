#include "itank.h"
#include "circleofpower.h"
#include "configuration.h"
#include "soundengine.h"

ITank::ITank(TankInfo tank_info, QString nick, int rank, ColorTeam color_team):QGraphicsObject()
{
    _tank_info = tank_info;

    _nick = nick;

    _rank = rank;

    _color_team = color_team;

    _max_live = _current_live = tank_info._live;

    _max_mana = _current_mana = _tank_info._mana;

    _base_armor = _tank_info._armor;

    _base_speed = _tank_info._speed;

    _base_live_regeneration = _tank_info._live_regeneration;

    _base_mana_regeneration = tank_info._mana_regeneration;

    _dead_time = MECH_DEAD_TIME_FOR_LEVEL;

    _is_enemy = false;

    _level = 1;

    _updates_available = 1;

    _current_experience = 0;

    _next_level_experience = MECH_INIT_NEXT_LEVEL_EXPERIENCE;

    _counter_millisec = 0;

    _animation_dead = getListOfPixmapFromStripImage(":/gt/sprites/tank_explotion.png",160);
    _frame_animation_dead = 0;
    _animation_level_up = getListOfPixmapFromStripImage(":/gt/sprites/tank_level_up.png",100);
    _frame_animation_level_up = 39;

    QTransform transformation;
    transformation.translate(-80, -80);
    _default_rect = transformation.mapRect(QRect(0, 0, 160, 160));

    _mini_map_tank = new QGraphicsPixmapItem(QPixmap(QString(":/gt/sprites/mini_map_tank_%1.png").arg(color_team)));
    _mini_map_tank->setTransformationMode(Qt::SmoothTransformation);
    _mini_map_tank->setTransformOriginPoint(_mini_map_tank->boundingRect().width()/2, _mini_map_tank->boundingRect().height()/2+2);
    _mini_map_tank->setZValue(20);

    _sound_explode = new SoundEngine(QUrl("qrc:/gt/sounds/explode.mp3"), MECH_MAX_DISTANCE_SOUND, this);
    _sound_level_up = new SoundEngine(QUrl("qrc:/gt/sounds/level_up.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    //añade un efecto de sombra ligero al tanque
    //    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //    shadow->setColor(Qt::black);
    //    shadow->setOffset(0);
    //    shadow->setBlurRadius(5);
    //    setGraphicsEffect(shadow);
}

ITank::~ITank()
{
    delete _skill_1;
    delete _skill_2;
    delete _skill_3;
    //****************
    delete _info_bar;
    delete _sound_explode;
    delete _sound_level_up;
}

TankInfo ITank::tankInfo() const
{
    return _tank_info;
}

ITank *ITank::principalReference()
{
    return _principal_reference;
}

void ITank::setPrincipalReference(ITank *tank)
{
    _principal_reference = tank;
    if(tank == this)
        _mini_map_tank->setPixmap(QPixmap(":/gt/sprites/mini_map_tank_0.png"));
    else
        _mini_map_tank->setPixmap(QPixmap(QString(":/gt/sprites/mini_map_tank_%1.png").arg(_color_team)));
}

void ITank::setCorner(int corner_vertical, int corner_horizontal)
{
    _corner[0] = corner_vertical;
    _corner[1] = corner_horizontal;
}

void ITank::createInfoBar()
{
    _info_bar = new InfoBar(_nick, _rank, _color_team);
    _info_bar->setZValue(100);
    scene()->addItem(_info_bar);
}

ColorTeam ITank::getColorTeam()
{
    return _color_team;
}

QString ITank::getName()
{
    return _tank_info._name;
}

QString ITank::getDescription()
{
    return _tank_info._description;
}

QPixmap ITank::getPreview()
{
    return _tank_info._preview;
}

QGraphicsPixmapItem *ITank::miniMapTank() const
{
    return _mini_map_tank;
}

void ITank::reborn()
{
    setStatus(NORMAL);
    _mini_map_tank->setVisible(true);

    _frame_animation_dead = 0;
    _frame_animation_level_up = 39;

    //se le asigna el ángulo inicial
    setRotation(0);
    setTurretRotation(0);

    setCurrentLive(_max_live, this, false);
    setCurrentMana(_max_mana, false);

    //se le asignan los estados del movimiento
    _move_forward = false;
    _move_backward = false;

    //se le asignan los estados de la rotación
    _rotate_left = false;
    _rotate_right = false;

    //indica que todos las habilidades están desactivadas
    setHurledSkill(1,false);
    setHurledSkill(2,false);
    setHurledSkill(3,false);

    _info_bar->setLiveBarData(_current_live, _max_live);
    _info_bar->setManaBarData(_current_mana, _max_mana);
    _info_bar->show();

    _last_tank_cause_damage = NULL;

    emit notifyReborn();
}

void ITank::addOtherCircleOfPower(CircleOfPower *circle_of_power)
{
    _others_circles_of_powers.append(circle_of_power);
}

CircleOfPower *ITank::circle_of_power() const
{
    return _circle_of_power;
}

void ITank::setCircleOfPower(CircleOfPower *circle_of_power)
{
    _circle_of_power = circle_of_power;
}

void ITank::dead()
{
    setStatus(DEAD);

    _mini_map_tank->setVisible(false);
    
    _sound_explode->playSound2D(pos(), principalReference()->pos(), Configuration::volumeSound());
    
    deadAnimation();
    
    //eliminar efectos
    removeAllEffects();

    _current_dead_time = _dead_time;
    _info_bar->hide();

    //adiciona experiencia a los enemigos
    if(_enemie.contains(_last_tank_cause_damage))
    {
        QList<ITank*> enemie = _last_tank_cause_damage->getAllies();
        int cant = _last_tank_cause_damage->getStatus() == DEAD? 0:1;
        int experience = _level * 12 + 100;
        foreach(ITank *killer, enemie)
            if(killer->getStatus() != DEAD && getDistance(pos(), killer->pos()) < 500)
                cant++;

        if(_last_tank_cause_damage->getStatus() != DEAD)
            _last_tank_cause_damage->addExperience(experience / cant);

        if(cant > 1)
            foreach(ITank *killer, enemie)
                if(killer->getStatus() != DEAD && getDistance(pos(), killer->pos()) < 500)
                    killer->addExperience(experience / cant);
    }
    //***************************************************

    notifyDead(_last_tank_cause_damage);
}

bool ITank::isEnemy()
{
    return _is_enemy;
}

void ITank::setIsEnemy(bool value)
{
    _is_enemy = value;
}

ISkillFactory *ITank::getSkill(int index)
{
    switch (index)
    {
        case 1:
        return _skill_1;
        case 2:
        return _skill_2;
        default:
        return _skill_3;
    }
}

bool ITank::isHurledSkill(int index)
{
    return _hurled_skills[index-1];
}

void ITank::setHurledSkill(int index, bool value)
{
    _hurled_skills[index-1] = value;
}

void ITank::setPos(QPointF pos)
{
    QGraphicsItem::setPos(pos);
    _info_bar->setPos(pos);
    if(_mini_map_tank->scene() && scene())
    {
        qreal x_p = QGraphicsObject::pos().x()/scene()->width()*100;
        qreal y_p = QGraphicsObject::pos().y()/scene()->height()*100;
        qreal x = _mini_map_tank->scene()->width()*x_p/100;
        qreal y = _mini_map_tank->scene()->height()*y_p/100;
        _mini_map_tank->setPos(x-_mini_map_tank->boundingRect().width()/2,y-_mini_map_tank->boundingRect().height()/2-2);
    }
    emit notifyPosition(pos);
}

void ITank::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}

double ITank::getRotation()
{
    int rot = (int)_angle%360;
    if(rot < 0)
        rot = 360+rot;
    return rot;
}

void ITank::setRotation(double angle)
{
    _angle = angle;
    QGraphicsItem::setRotation(_angle);
    _mini_map_tank->setRotation(_angle);
    emit notifyRotation(angle);
}

double ITank::turretRotation()
{
    return _turret_rotation;
}

void ITank::setTurretRotation(qreal turret_angle)
{
    if(_status == NORMAL)
    {
        _turret_rotation = turret_angle;
        emit notifyRotationTurret(_turret_rotation);
    }
}

int ITank::currentLive()
{
    return _current_live;
}

void ITank::setCurrentLive(double current_live, ITank *tank_cause_damage, bool animate_numbers)
{
    double old_live = _current_live;

    if(current_live <= 0)
    {
        _last_tank_cause_damage = tank_cause_damage;

        _current_live = 0;

        dead();

        return;
    }
    else if(current_live > _max_live)
        current_live = _max_live;
    else if(current_live < _current_live)
        _last_tank_cause_damage = tank_cause_damage;

    if(current_live == _current_live)
        return;

    _current_live = current_live;

    if(animate_numbers)
    {
        int number = _current_live - old_live;
        ColorNumber * color_number = new ColorNumber(LIVE,number);
        int center_y = color_number->boundingRect().height()/2;

        color_number->setPos(QPointF(pos().x()+ 40, pos().y() - center_y - 32));
        color_number->setZValue(1000);

        scene()->addItem(color_number);
    }
    _info_bar->setLiveBarData(_current_live, _max_live);

    emit notifyCurrentLive(_current_live);
}

int ITank::maxLive()
{
    return _max_live;
}

void ITank::setMaxLive(double max_live)
{
    bool equal = _max_live == _current_live;
    _max_live = max_live;
    if(_max_live < _current_live || equal)
        setCurrentLive(_max_live, this, false);

    _info_bar->setLiveBarData(_current_live, _max_live);
    emit notifyMaxLive(max_live);
}

int ITank::currentMana()
{
    return _current_mana;
}

void ITank::setCurrentMana(double current_mana, bool animate_numbers)
{
    double old_mana = _current_mana;

    if(current_mana < 0)
        current_mana = 0;
    else if(current_mana > _max_mana)
        current_mana = _max_mana;

    if(_current_mana == current_mana)
        return;

    _current_mana = current_mana;

    if(animate_numbers)
    {
        int number = _current_mana - old_mana;
        ColorNumber * color_number = new ColorNumber(MANA,number);
        int center_y = color_number->boundingRect().height()/2;

        color_number->setPos(QPointF(pos().x()+ 40, pos().y() - center_y - 22));
        color_number->setZValue(1000);

        scene()->addItem(color_number);
    }
    _info_bar->setManaBarData(_current_mana, _max_mana);

    emit notifyCurrentMana(current_mana);
}

int ITank::maxMana()
{
    return _max_mana;
}

void ITank::setMaxMana(double max_mana)
{
    bool equal = _max_mana == _current_mana;
    _max_mana = max_mana;
    if(_max_mana < _current_mana || equal)
        setCurrentMana(_max_mana, false);

    _info_bar->setManaBarData(_current_mana, _max_mana);
    emit notifyMaxMana(max_mana);
}

int ITank::getBaseLiveRegeneration()
{
    return _base_live_regeneration;
}

int ITank::getBonusLiveRegeneration()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusLiveRegeneration();
    return value;
}

int ITank::getTotalLiveRegeneration()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusLiveRegeneration();

    return _base_live_regeneration + value;
}

int ITank::getBaseManaRegeneration()
{
    return _base_mana_regeneration;
}

int ITank::getBonusManaRegeneration()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusManaRegeneration();
    return value;
}

int ITank::getTotalManaRegeneration()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusManaRegeneration();

    return _base_mana_regeneration + value;
}

int ITank::baseArmor()
{
    return _base_armor;
}

int ITank::getBonusArmor()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusArmor();
    return value;
}

int ITank::getTotalArmor()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusArmor();

    return _base_armor + value;
}

double ITank::baseSpeed()
{
    return _base_speed;
}

double ITank::getBonusSpeed()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusSpeed();
    return value;
}

double ITank::getTotalSpeed()
{
    double value = 0;
    foreach(Effect *effect, _effects)
        value += effect->bonusSpeed();

    double total_speed = _base_speed + value;
    if(total_speed < MECH_MIN_TANK_SPEED)
        return MECH_MIN_TANK_SPEED;
    else if(total_speed > MECH_MAX_TANK_SPEED)
        return MECH_MAX_TANK_SPEED;
    return total_speed;
}

Tank_Status ITank::getStatus()
{
    return _status;
}

void ITank::setStatus(Tank_Status value)
{
    _status = value;
    emit notifyStatus(value);
}

int ITank::getCurrentExperience()
{
    return _current_experience;
}

int ITank::getNextLevelExperience()
{
    return _next_level_experience;
}

void ITank::addExperience(int value)
{
    if(_level < MECH_MAX_TANK_LEVEL)
    {
        _current_experience += value;
        while(_current_experience >= _next_level_experience)
        {
            levelUp();
        }
        if(_level == MECH_MAX_TANK_LEVEL)
        {
            _next_level_experience -= 100 * _level;
            _current_experience = _next_level_experience;
        }
        emit notifyExperience(_current_experience, _next_level_experience);
    }
}

void ITank::levelUp()
{
    if(_level < MECH_MAX_TANK_LEVEL)
    {
        _level++;

        _sound_level_up->playSound2D(pos(), principalReference()->pos(), Configuration::volumeSound());

        _updates_available++;

        _frame_animation_level_up = 0;

        _current_experience -= _next_level_experience;

        _next_level_experience = 100 * _level;

        setMaxLive(_max_live + _tank_info._inc_max_live);
        setMaxMana(_max_mana + _tank_info._inc_max_mana);

        _base_live_regeneration += _tank_info._inc_live_regeneration;
        _base_mana_regeneration += _tank_info._inc_mana_regeneration;
        _base_armor += _tank_info._inc_armor;
        _base_speed += _tank_info._inc_speed;
        _dead_time += MECH_DEAD_TIME_FOR_LEVEL;

        emit notifyLevelUp(_level);
    }
}

bool ITank::inLevelUpAnimation()
{
    return _frame_animation_level_up <= 38;
}

void ITank::levelUpAnimation()
{
    if(inLevelUpAnimation())
    {
        _frame_animation_level_up++;
        update(boundingRect());
    }
}

bool ITank::inDeadAnimation()
{
    return _status == DEAD && _frame_animation_dead <= 14;
}

void ITank::deadAnimation()
{
    if(inDeadAnimation())
    {
        _frame_animation_dead++;
        update(boundingRect());
    }
}

int ITank::getLevel()
{
    return _level;
}

int ITank::getUpdatesAvailablesForSkills()
{
    return _updates_available;
}

void ITank::updateSkill(int index)
{
    if(_updates_available > 0 && _status != DEAD)
    {
        switch (index)
        {
            case 1:
                if(_skill_1->isReadyForUpdate())
                {
                    _skill_1->levelUp();
                    _updates_available--;
                    emit notifySkillLevelUp(1, _skill_1->getLevel());
                }
            break;
            case 2:
                if(_skill_2->isReadyForUpdate())
                {
                    _skill_2->levelUp();
                    _updates_available--;
                    emit notifySkillLevelUp(2, _skill_2->getLevel());
                }
            break;
            case 3:
                if(_skill_3->isReadyForUpdate())
                {
                    _skill_3->levelUp();
                    _updates_available--;
                    emit notifySkillLevelUp(3, _skill_3->getLevel());
                }
        }

    }
}

void ITank::activeSkill(int index)
{
    if(_updates_available > 0)
    {
        switch (index)
        {
            case 1:
                _skill_1->active();
            break;
            case 2:
                _skill_2->active();
            break;
            default:
                _skill_3->active();
        }
        _updates_available--;
    }
}

int ITank::getDeadTime()
{
    return _dead_time;
}

void ITank::setDeadTime(int dead_time)
{
    _dead_time = dead_time;
}

bool ITank::isMoveForward()
{
    return _move_forward;
}

void ITank::setMoveForward(bool value)
{
    _move_forward = value;
}

bool ITank::isMoveBackward()
{
    return _move_backward;
}

void ITank::setMoveBackward(bool value)
{
    _move_backward = value;
}

bool ITank::isRotateLeft()
{
    return _rotate_left;
}

void ITank::setRotateLeft(bool value)
{
    _rotate_left = value;
}

bool ITank::isRotateRight()
{
    return _rotate_right;
}

void ITank::setRotateRight(bool value)
{
    _rotate_right = value;
}

QList<ITank *> ITank::getAllies()
{
    return _allies;
}

QList<ITank *> ITank::getEnemie()
{
    return _enemie;
}

QList<ITank *> ITank::getOthersTanks()
{
    return _others_tanks;
}

void ITank::addAllied(ITank *tank)
{
    _allies.append(tank);
    _others_tanks.append(tank);
}

void ITank::addEnemy(ITank *tank)
{
    _enemie.append(tank);
    _others_tanks.append(tank);
}

void ITank::setObstructecAreas(QList<QPainterPath> areas)
{
    _obstructed_areas = areas;
}

QRectF ITank::boundingRect() const
{
    return _default_rect;
}

void ITank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(_status != DEAD)
    {
        painter->save();
        if(_is_enemy)
            paintForEnemies(painter,option,widget);
        else
            paintForFriends(painter,option,widget);
        painter->restore();

        if(inLevelUpAnimation())
            painter->drawPixmap(-50,-50,_animation_level_up.at(_frame_animation_level_up));
    }
    else if(inDeadAnimation())
        painter->drawPixmap(-80,-80,_animation_dead.at(_frame_animation_dead));
}

void ITank::addEffect(Effect *effect)
{
    _effects.append(effect);
    scene()->addItem(effect);
    emit notifyEffectChange();
}

void ITank::removeEffect(Effect *effect)
{
    _effects.removeAll(effect);
    effect->destroy();
    emit notifyEffectChange();
}

void ITank::removeAllEffects()
{
    while(!_effects.isEmpty())
    {
        Effect* effect = _effects.at(0);
        _effects.removeAt(0);
        effect->destroy();
    }
    emit notifyEffectChange();
}

QList<QPainterPath> ITank::getObstructedAreas()
{
    return _obstructed_areas;
}

void ITank::initTank()
{
    reborn();
}

void ITank::move()
{
    QPointF old_pos = pos();
    QPointF current_pos = pos();
    double angle_rad = _angle * PIx2 / 360;
    double speed = getTotalSpeed()/10.0;
    double cosangle_s = cos( angle_rad ) * speed;
    double sinangle_s = sin( angle_rad ) * speed;

    if(_move_forward)
        current_pos += QPointF(sinangle_s, -cosangle_s);
    if(_move_backward)
        current_pos += QPointF(-sinangle_s, cosangle_s);

    setPos(current_pos);
    if(isOutOfCorner() || collideWithSomeThing())
        setPos(old_pos);
}

void ITank::rotate()
{
    double old_angle = _angle;
    double speed = getTotalSpeed()/10.0;
    if(_rotate_right)
        _angle += speed;
    if(_rotate_left)
        _angle -= speed;

    setRotation(_angle);
    if(collideWithSomeThing())
        setRotation(old_angle);
}

void ITank::executeSkill(int index)
{
    if(_status != DEAD)
    {
        _ready_to_execute_skill = false;
        _counter_delay_beteen_skills = 0;
        getSkill(index)->executeSkill();
    }
}

bool ITank::collideWithSomeThing()
{
    if(collidesWithObstructedAreas())
        return true;

    foreach (ITank* tank, _others_tanks)
        if(tank->getStatus() != DEAD && collidesWithItem(tank))
            return true;

    foreach (CircleOfPower* circle_of_power, _others_circles_of_powers)
        if(collidesWithItem(circle_of_power))
            return true;

    return false;
}

bool ITank::collidesWithObstructedAreas()
{
    QTransform transformation = sceneTransform();
    foreach (QPainterPath area, _obstructed_areas)
        if(area.intersects(transformation.map(shape())))
            return true;
    return false;
}

bool ITank::isOutOfCorner()
{
    return pos().x() < 0 || pos().x() > _corner[0] || pos().y() < 0 || pos().y() > _corner[1];
}


void ITank::advance(int phase)
{
    if(!phase)
        return;

    if(_skill_1->inCooldown())
        _skill_1->updateCooldown();
    if(_skill_2->inCooldown())
        _skill_2->updateCooldown();
    if(_skill_3->inCooldown())
        _skill_3->updateCooldown();


    bool second_pass = false;

    _counter_millisec += TICK;
    if(_counter_millisec >= ONE_SECOND)
    {
        _counter_millisec -= ONE_SECOND;
        second_pass = true;
    }

    if(!_ready_to_execute_skill)
    {
        _counter_delay_beteen_skills += TICK;
        if(_counter_delay_beteen_skills > DELAY_BETWEEN_SKILLS)
            _ready_to_execute_skill = true;
    }

    if(_status == DEAD)
    {
        _current_dead_time -= TICK;

        if(second_pass)
            notifyTimeToReborn(_current_dead_time/1000);

        if(inDeadAnimation())
            deadAnimation();
        else if(_current_dead_time <= 0)
            reborn();
    }
    else
    {
        if(inLevelUpAnimation())
            levelUpAnimation();

        if(second_pass)
        {
            int live_regeneration = _base_live_regeneration;
            int mana_regeneration = _base_mana_regeneration;
            int min = 0;

            bool changes = false;
            for(int i = 0; i < _effects.size(); ++i)
            {
                Effect *effect = _effects.at(i);
                if(effect->stunned() && _status ==NORMAL)
                    setStatus(STUNNED);

                live_regeneration += effect->bonusLiveRegeneration();
                mana_regeneration += effect->bonusManaRegeneration();
                if(effect->bonusLiveRegeneration() < min)
                {
                    _last_tank_cause_damage = effect->creatorTank();
                    min = effect->bonusLiveRegeneration();
                }
                if(!effect->isForever())
                {
                    if(!effect->nextTime())
                    {
                        if(effect->stunned())
                            setStatus(NORMAL);

                        removeEffect(effect);
                        --i;
                        changes = true;
                    }
                }
            }
            if(changes)
                emit notifyEffectChange();

            setCurrentLive(_current_live + live_regeneration, _last_tank_cause_damage);
            setCurrentMana(_current_mana + mana_regeneration);

            if(_status == DEAD)
                return;
        }

        if(_status == NORMAL)
        {
            if(_move_forward != _move_backward)
                move();
            if(_rotate_left != _rotate_right)
                rotate();

            if(_ready_to_execute_skill && isHurledSkill(1) && _skill_1->isReadyToExecute())
                executeSkill(1);
            if(_ready_to_execute_skill && isHurledSkill(2) && _skill_2->isReadyToExecute())
                executeSkill(2);
            if(_ready_to_execute_skill && isHurledSkill(3) && _skill_3->isReadyToExecute())
                executeSkill(3);
        }

    }

    gameLoop();
}
