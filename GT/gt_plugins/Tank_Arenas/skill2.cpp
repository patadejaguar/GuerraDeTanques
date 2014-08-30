#include "skill2.h"
#include "itank.h"
#include "configuration.h"

Skill2::Skill2(ITank *tank, int bonus_armor, int time_effect, QList<QPixmap> pixmaps_armor) :
    ISkill(tank)
{
    _time_effect = time_effect;

    _step = 0;

    _milliseconds = 0;

    _pixmaps_armor = pixmaps_armor;

    _bounding_rect = _pixmaps_armor.at(0).rect();

    connect(_tank, &ITank::notifyPosition, this, &Skill2::setPosition);
    connect(_tank, SIGNAL(notifyDead(ITank*)), this, SLOT(destroy()));

    Effect* effect = new Effect(_tank, _tank, 0, 0, bonus_armor, 0, false, false, time_effect, QList<QPixmap>());
    _tank->addEffect(effect);
}

void Skill2::setPosition(QPointF pos)
{
    QGraphicsItem::setPos(pos);
}

QPainterPath Skill2::shape() const
{
    return _shape;
}

QRectF Skill2::boundingRect() const
{
    return _bounding_rect.translated(-50,-50);
}

void Skill2::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-50,-50,_pixmaps_armor.at(_step));
}

void Skill2::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter, option, widget);
}

void Skill2::gameLoop()
{
    bool second_pass = false;

    _milliseconds += TICK;
    if(_milliseconds >= 1000)
    {
        _milliseconds -= 1000;
        second_pass = true;
    }
    if(second_pass)
        if(--_time_effect <= 0)
            destroy();

    _step = (_step + 1) % _pixmaps_armor.size();
}

Skill2Factory::Skill2Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 9000, 25)
{
    _bonus_armor = 8;

    _time_effect = 5;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_arenas/sounds/shield.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps_armor = getListOfPixmapFromStripImage(":xavier_tank_arenas/sprites/skill_2.png",100);
}

QString Skill2Factory::getDetails()
{
    QList<QString> controls = Configuration::controlsName();
    QString description = QString("<span class = \"b\">%1 %2</span><span> - </span><span class = \"o\">[%3]</span> - <span class = \"b\">[%4]+[%5]</span><span><br/><br/>").arg(getName()).arg(getLevel()).arg(getTankLevelForUpdate()!=-1?QString("%1").arg(getTankLevelForUpdate()):"").arg(controls.at(5)).arg(controls.at(8));

    description.append(QString("<span>%1</span><br/>").arg(getDescription()));

    if(isActive())
    {
        QString details = QString("<table>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Armadura: </p></td>"
                                  "<td><p class=\"yl\" align=\"justify\">%1</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%3 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_bonus_armor).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _bonus_armor = 10;
            _mana_cost = 27;
            _cooldown = 8000;
        break;
        case 3:
            _bonus_armor = 13;
            _mana_cost = 29;
            _cooldown = 7000;
        break;
        case 4:
            _bonus_armor = 17;
            _mana_cost = 31;
            _cooldown = 6000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _bonus_armor, _time_effect, _pixmaps_armor);
    skill->setZValue(_tank->zValue()+0.1);
    skill->setPos(_tank->pos());

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
