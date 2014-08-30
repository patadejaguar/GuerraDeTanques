#include "skill3.h"
#include "itank.h"
#include "configuration.h"
#include "myeffect.h"

Skill3::Skill3(ITank *tank, int bonus_armor, int time_effect, QList<QPixmap> pixmaps_armor) :
    ISkill(tank)
{
    Effect* effect = new MyEffect(_tank, _tank, 0, 0, bonus_armor, 0, false, false, time_effect, pixmaps_armor);
    _tank->addEffect(effect);

    foreach(ITank* allie, _tank->getAllies())
    {
        if(getDistance(_tank->pos(), allie->pos()) <= 300)
        {
            Effect* effect = new MyEffect(_tank, allie, 0, 0, bonus_armor, 0, false, false, time_effect, pixmaps_armor);
            allie->addEffect(effect);
        }
    }
}

void Skill3::setPosition(QPointF pos)
{
    QGraphicsItem::setPos(pos);
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
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);    
}

void Skill3::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter, option, widget);
}

void Skill3::gameLoop()
{
    destroy();
}

Skill3Factory::Skill3Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 40000, 100)
{
    _bonus_armor = 50;

    _time_effect = 4;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_nemo/sounds/shield.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps = getListOfPixmapFromStripImage(":xavier_tank_nemo/sprites/skill_3.png", 106);
}

QString Skill3Factory::getDetails()
{
    QList<QString> controls = Configuration::controlsName();
    QString description = QString("<span class = \"b\">%1 %2</span><span> - </span><span class = \"o\">[%3]</span> - <span class = \"b\">[%4][%5]</span><span><br/><br/>").arg(getName()).arg(getLevel()).arg(getTankLevelForUpdate()!=-1?QString("%1").arg(getTankLevelForUpdate()):"").arg(controls.at(6)).arg(controls.at(9));

    description.append(QString("<span>%1</span><br/>").arg(getDescription()));

    if(isActive())
    {
        QString details = QString("<table>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Armadura: </p></td>"
                                  "<td><p class=\"yl\" align=\"justify\">%1</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de efecto: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%3</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%4 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_bonus_armor).arg(_time_effect).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill3Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _bonus_armor = 70;
            _mana_cost = 150;
            _time_effect = 6;
            _cooldown = 50000;
        break;
        case 3:
            _bonus_armor = 90;
            _mana_cost = 200;
            _time_effect = 8;
            _cooldown = 60000;
        break;
    }
}

ISkill *Skill3Factory::createSkill()
{
    ISkill* skill = new Skill3(_tank, _bonus_armor, _time_effect, _pixmaps);

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
