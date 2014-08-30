﻿#include "skill3.h"
#include "tank.h"
#include "circleofpower.h"
#include "configuration.h"

Skill3::Skill3(ITank *tank, double damage, QPointF turret_pos, int turret_angle, QList<QPixmap> pixmaps_shot) :
    ISkill(tank)
{
    _damage = damage;

    _angle = turret_angle;

    _explode = false;

    _velocity = 12;

    _distance = 500;

    _step = 0;

    _pixmaps_shot = pixmaps_shot;

    _bounding_rect = QRectF(-12,-12,24,24);

    _shape.addEllipse(-4, -4, 8, 8);

    setPos(turret_pos.x(),turret_pos.y()-24);

    setTransformOriginPoint(mapFromScene(turret_pos));

    setRotation(_angle);

    _sound = new SoundEngine(QUrl("qrc:/xavier_tank_nostradamus/sounds/shot_explode.mp3"),MECH_MAX_DISTANCE_SOUND, this);
}

QPainterPath Skill3::shape() const
{    
    return _shape;
}

QRectF Skill3::boundingRect() const
{
    return _bounding_rect;
}

void Skill3::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(_explode)
        painter->drawPixmap(-12,-12,_pixmaps_shot.at(_step));
    else
        painter->drawPixmap(-12,-12,_pixmaps_shot.at(0));
}

void Skill3::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter,option,widget);
}

void Skill3::gameLoop()
{
    if(_explode)
    {
        _step++;
        if(_step==14)
            destroy();
    }
    else if(++_step<_distance/_velocity)
    {
        double angle_rad = _angle * PIx2 / 360;
        double cosangle = cos( angle_rad );
        double sinangle = sin( angle_rad );
        setPos(x()-sinangle*-_velocity, y()+cosangle*-_velocity);

        foreach (QPainterPath path, _tank->getObstructedAreas())
        {
            if(path.intersects(sceneTransform().map(shape())))
            {
                explod();
                return;
            }
        }

        foreach (ITank* enemy, _tank->getEnemie())
        {
            if(enemy->circle_of_power()->isActive() && collidesWithItem(enemy->circle_of_power()))
            {
                explod();
                enemy->circle_of_power()->addDamage(_damage*(enemy->maxMana()-enemy->currentMana()), _tank);
                return;
            }
            else if(enemy->getStatus() != DEAD && collidesWithItem((QGraphicsItem*)enemy))
            {
                explod();

                int current_live = enemy->currentLive() + enemy->getTotalArmor() - _damage*(enemy->maxMana()-enemy->currentMana());
                if(current_live < enemy->currentLive())
                {
                    enemy->setCurrentLive(current_live, _tank);
                }
                return;
            }
        }

        foreach(ITank* allie, _tank->getAllies())
            if(allie->getStatus() != DEAD && collidesWithItem((QGraphicsItem*)allie))
                explod();
    }
    else
        explod();
}

void Skill3::explod()
{
    _explode= true;
    _step = 1;

    _sound->playSound2D(pos(), _tank->principalReference()->pos(), Configuration::volumeSound());

}

Skill3Factory::Skill3Factory(ITank *tank, SkillInfo skill_info):
    ISkillFactory(tank, skill_info, 30000, 110)
{
    _damage = 0.5;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_nostradamus/sounds/shot.mp3"),MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps_shot = getListOfPixmapFromStripImage(":xavier_tank_nostradamus/sprites/skill_3.png",24);

    _pixmaps_shot_fire = getListOfPixmapFromStripImage(":xavier_tank_nostradamus/sprites/shot_fire_skill_3.png",30);
}

QString Skill3Factory::getDetails()
{
    QList<QString> controls = Configuration::controlsName();
    QString description = QString("<span class = \"b\">%1 %2</span><span> - </span><span class = \"o\">[%3]</span> - <span class = \"b\">[%4]+[%5]</span><span><br/><br/>").arg(getName()).arg(getLevel()).arg(getTankLevelForUpdate()!=-1?QString("%1").arg(getTankLevelForUpdate()):"").arg(controls.at(6)).arg(controls.at(9));

    description.append(QString("<span>%1</span><br/>").arg(getDescription()));

    if(isActive())
    {
        QString details = QString("<table>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Daño: </p></td>"
                                  "<td><p class=\"rl\" align=\"justify\">%1 por punto de mana perdido</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%3 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_damage,0,'f',1).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill3Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _damage = 0.7;
            _mana_cost = 150;
            _cooldown = 35000;
        break;
        case 3:
            _damage = 0.9;
            _mana_cost = 170;
            _cooldown = 40000;
        break;
    }
}

ISkill *Skill3Factory::createSkill()
{
    QPointF turret_pos = _tank->mapToScene(((Tank*)_tank)->getTurretPosition().x() + 11, ((Tank*)_tank)->getTurretPosition().y() + 24);
    int angle = _tank->turretRotation();

    //creo la habilidad
    ISkill* skill = new Skill3(_tank, _damage, turret_pos, angle, _pixmaps_shot);

    //efecto de fuego.
    SingleAnimation *_shot_fire = new SingleAnimation(turret_pos, angle, _pixmaps_shot_fire);
    _tank->scene()->addItem(_shot_fire);

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}