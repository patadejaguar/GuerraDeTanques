#include "skill3.h"
#include "itank.h"
#include "configuration.h"
#include "myeffect.h"

Skill3::Skill3(ITank *tank, int time_effect, QList<QPixmap> pixmaps, QList<QPixmap> frozen_effect):
    ISkill(tank)
{

    _step = 0;

    _time_effect = time_effect;

    _pixmaps = pixmaps;

    _bounding_rect = _pixmaps.at(0).rect();

    _frozen_effect = frozen_effect;

    connect(_tank, &ITank::notifyPosition, this, &Skill3::setPosition);
    connect(_tank, SIGNAL(notifyDead(ITank*)), this, SLOT(destroy()));

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
    return _bounding_rect.translated(-150, -150);
}

void Skill3::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);    
    painter->drawPixmap(-150, -150, _pixmaps.at(_step));
}

void Skill3::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter, option, widget);
}

void Skill3::gameLoop()
{
    foreach(ITank* enemy, _tank->getEnemie())
    {
        double distance = getDistance(_tank->pos(), enemy->pos());
        if(!_effects.keys().contains(enemy) && distance <= 150*((double)_step/11.0))
        {
            Effect* effect = new MyEffect(_tank, enemy, -25, 0, 0, 0, true, false, _time_effect, _frozen_effect);
            SoundEngine* sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_escarcha/sounds/frost.mp3"), MECH_MAX_DISTANCE_SOUND, effect);
            sound_engine->playSound2D(enemy->pos(), _tank->principalReference()->pos(), Configuration::volumeSound());
            enemy->addEffect(effect);
            _effects.insert(enemy, effect);
        }
    }

    if(++_step >= _pixmaps.size())
    {
        _step = _pixmaps.size()-1;
        destroy();
    }
}

Skill3Factory::Skill3Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 30000, 150)
{
    _time_effect = 4;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_escarcha/sounds/wave.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps = getListOfPixmapFromStripImage(":xavier_tank_escarcha/sprites/skill_3.png", 300);

    _frozen_effect = getListOfPixmapFromStripImage(":xavier_tank_escarcha/sprites/effect_skill_3.png", 90);
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
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de efecto: </p></td>"
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
                                  "</table>").arg(_time_effect).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill3Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _mana_cost = 200;
            _time_effect = 5;
            _cooldown = 50000;
        break;
        case 3:
            _mana_cost = 250;
            _time_effect = 6;
            _cooldown = 70000;
        break;
    }
}

ISkill *Skill3Factory::createSkill()
{
    ISkill* skill = new Skill3(_tank, _time_effect, _pixmaps, _frozen_effect);
    skill->setZValue(_tank->zValue()+0.1);
    skill->setPos(_tank->pos());

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
