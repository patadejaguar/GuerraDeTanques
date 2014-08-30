#include "skill2.h"
#include "itank.h"
#include "configuration.h"

Skill2::Skill2(ITank *tank, int bonus_mana, QList<QPixmap> pixmaps) :
    ISkill(tank)
{
    _step = 0;

    _pixmaps = pixmaps;

    _bounding_rect = _pixmaps.at(0).rect();

    connect(_tank, &ITank::notifyPosition, this, &Skill2::setPosition);
    connect(_tank, SIGNAL(notifyDead(ITank*)), this, SLOT(destroy()));

    _tank->setCurrentMana(_tank->currentMana()+bonus_mana);

    foreach(ITank* allie, _tank->getAllies())
        if(getDistance(_tank->pos(), allie->pos()) <= 500 && allie->getStatus() != DEAD)
            allie->setCurrentMana(allie->currentMana()+bonus_mana);
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
    return _bounding_rect.translated(-115, -115);
}

void Skill2::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-115, -115, _pixmaps.at(_step));
}

void Skill2::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter, option, widget);
}

void Skill2::gameLoop()
{
    if(++_step >= _pixmaps.size())
    {
        _step = _pixmaps.size()-1;
        destroy();
    }
}

Skill2Factory::Skill2Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 30000, 20)
{
    _bonus_mana = 60;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_nemo/sounds/elixir.mp3"),MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps = getListOfPixmapFromStripImage(":xavier_tank_nemo/sprites/skill_2.png", 230);
}

QString Skill2Factory::getDetails()
{
    QList<QString> controls = Configuration::controlsName();
    QString description = QString("<span class = \"b\">%1 %2</span><span> - </span><span class = \"o\">[%3]</span> - <span class = \"b\">[%4][%5]</span><span><br/><br/>").arg(getName()).arg(getLevel()).arg(getTankLevelForUpdate()!=-1?QString("%1").arg(getTankLevelForUpdate()):"").arg(controls.at(5)).arg(controls.at(8));

    description.append(QString("<span>%1</span><br/>").arg(getDescription()));

    if(isActive())
    {
        QString details = QString("<table>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Reg de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%1</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%3 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_bonus_mana).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _bonus_mana = 80;
            _mana_cost = 40;
            _cooldown = 30000;
        break;
        case 3:
            _bonus_mana = 120;
            _mana_cost = 60;
            _cooldown = 30000;
        break;
        case 4:
            _bonus_mana = 150;
            _mana_cost = 80;
            _cooldown = 30000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _bonus_mana, _pixmaps);
    skill->setZValue(_tank->zValue()+0.1);
    skill->setPos(_tank->pos());

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
