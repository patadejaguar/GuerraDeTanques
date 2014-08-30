#include "skill2.h"
#include "itank.h"
#include "configuration.h"

Skill2::Skill2(ITank *tank, int time_effect) :
    ISkill(tank)
{
    _tank = tank;

    _object = new ObjectFactory();

    IObject* obj = _object->createInstance(time_effect, tank);
    obj->setZValue(tank->zValue()-0.1);

    _tank->scene()->addItem(obj);
    _tank->miniMapTank()->scene()->addItem(obj->getMiniMapView());

    obj->setPos(tank->pos());

}

Skill2::~Skill2()
{
   delete  _object;
}

QPainterPath Skill2::shape() const
{
    return _shape;
}

QRectF Skill2::boundingRect() const
{
    return _bounding_rect;
}

void Skill2::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

void Skill2::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    Q_UNUSED(painter);
}

void Skill2::gameLoop()
{
    destroy();
}

Skill2Factory::Skill2Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 10000, 25)
{
    _time_effect = 2;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_viuda_negra/sounds/tramp.mp3"), MECH_MAX_DISTANCE_SOUND, this);
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

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _time_effect = 3;
            _mana_cost = 27;
            _cooldown = 13000;
        break;
        case 3:
            _time_effect = 4;
            _mana_cost = 29;
            _cooldown = 15000;
        break;
        case 4:
            _time_effect = 6;
            _mana_cost = 31;
            _cooldown = 18000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _time_effect);

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
