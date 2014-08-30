#include "skill2.h"
#include "itank.h"
#include "configuration.h"

Skill2::Skill2(ITank *tank, int mana_drain, QList<QPixmap> pixmaps_drain) :
    ISkill(tank)
{
    _mana_drain = mana_drain;
    _pixmaps_drain = pixmaps_drain;
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
    paintForFriends(painter, option, widget);
}

void Skill2::gameLoop()
{
    foreach(ITank* enemy, _tank->getEnemie())
        if(getDistance(_tank->pos(), enemy->pos()) <= 400 && enemy->getStatus() != DEAD)
        {
            int mana = enemy->currentMana();
            enemy->setCurrentMana(enemy->currentMana()-_mana_drain);
            Drains* drains = new Drains(_tank, mana-enemy->currentMana(), _pixmaps_drain);
            drains->setPos(enemy->pos());
            scene()->addItem(drains);
        }
    destroy();
}

Skill2Factory::Skill2Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 10000, 40)
{
    _mana_drain = 40;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_nostradamus/sounds/mana_drain.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    _pixmaps_drain = getListOfPixmapFromStripImage(":xavier_tank_nostradamus/sprites/skill_2.png", 20);
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
                                  "<td><p class=\"w\" align=\"justify\">Drenado de mana: </p></td>"
                                  "<td><p class=\"b\" align=\"justify\">%1</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%3 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_mana_drain).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _mana_drain = 50;
            _mana_cost = 50;
            _cooldown = 12000;
        break;
        case 3:
            _mana_drain = 60;
            _mana_cost = 60;
            _cooldown = 14000;
        break;
        case 4:
            _mana_drain = 70;
            _mana_cost = 70;
            _cooldown = 16000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _mana_drain, _pixmaps_drain);

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
