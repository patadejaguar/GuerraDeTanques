#include "skill2.h"
#include "itank.h"
#include "configuration.h"
#include "myeffect.h"

Skill2::Skill2(ITank *tank, int live_regen, QList<QPixmap> pixmaps, SoundEngine *sound_engine):
    ISkill(tank)
{
    _step = 0;

    _pixmaps = pixmaps;

    _bounding_rect = _pixmaps.at(0).rect();

    _milliseconds = 0;

    _sound_engine = sound_engine;

    connect(_tank, &ITank::notifyPosition, this, &Skill2::setPosition);
    connect(_tank, SIGNAL(notifyDead(ITank*)), this, SLOT(destroy()));

    _tank->setCurrentLive(_tank->currentLive()+live_regen, _tank);
}

Skill2::~Skill2()
{
    _sound_engine->stopSound();
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
    return _bounding_rect.translated(-48, -48);
}

void Skill2::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-48, -48, _pixmaps.at(_step));
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
    :ISkillFactory(tank, skill_info, 12000, 18)
{
    _live_regen = 20;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_pantanoso/sounds/leaf.mp3"),MECH_MAX_DISTANCE_SOUND, this);
    _sound_engine->mediaPlayer()->playlist()->setPlaybackMode(QMediaPlaylist::Loop);

    _pixmaps = getListOfPixmapFromStripImage(":xavier_tank_pantanoso/sprites/skill_2.png", 96);
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
                                  "<td><p class=\"w\" align=\"justify\">Regeneración de vida: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%1</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%3</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%4 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_live_regen).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _live_regen = 40;
            _mana_cost = 22;
            _cooldown = 16000;
        break;
        case 3:
            _live_regen = 60;
            _mana_cost = 26;
            _cooldown = 20000;
        break;
        case 4:
            _live_regen = 80;
            _mana_cost = 30;
            _cooldown = 24000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _live_regen, _pixmaps, _sound_engine);
    skill->setZValue(_tank->zValue()+0.1);
    skill->setPos(_tank->pos());

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
