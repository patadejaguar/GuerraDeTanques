#include "skill2.h"
#include "itank.h"
#include "configuration.h"
#include "myeffect.h"

Skill2::Skill2(ITank *tank, int slow_down_percent, int time_effect, QList<QPixmap> pixmaps, QList<QPixmap> aura, SoundEngine *sound_engine):
    ISkill(tank)
{
    _step = 0;

    _pixmaps = pixmaps;

    _aura = aura;

    _bounding_rect = _pixmaps.at(0).rect();

    _time_effect = time_effect;

    _slow_down_percent = slow_down_percent;

    _milliseconds = 0;

    _sound_engine = sound_engine;
}

Skill2::~Skill2()
{
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
    return _bounding_rect.translated(-150, -150);
}

void Skill2::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-150, -150, _pixmaps.at(_step));
}

void Skill2::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter, option, widget);
}

void Skill2::gameLoop()
{
    foreach(ITank* enemy, _tank->getEnemie())
    {
        double distance = getDistance(_tank->pos(), enemy->pos());
        if(!_effects.keys().contains(enemy) && distance <= 160 && enemy->getStatus() != DEAD)
        {
            int slow = (double)enemy->tankInfo()._speed/100*_slow_down_percent;
            if(slow > 0)
                slow *= -1;
            Effect* effect = new MyEffect(_tank, enemy, 0, 0, 0, slow, false, false, _time_effect, _aura, -0.1);
            enemy->addEffect(effect);
            _effects.insert(enemy, effect);
        }
        else if(distance > 160 && enemy->getStatus() != DEAD)
        {
            Effect* effect = _effects.value(enemy, NULL);
            if(effect != NULL)
            {
                (dynamic_cast<MyEffect*>(effect))->setTimeZero();
                _effects.remove(enemy);
            }

        }
    }

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

    _step = (_step + 1) % _pixmaps.size();

}

Skill2Factory::Skill2Factory(ITank *tank, SkillInfo skill_info)
    :ISkillFactory(tank, skill_info, 15000, 22)
{
    _slow_down_percent = 10;

    _time_effect = 5;

    _sound_engine = new SoundEngine(QUrl("qrc:/xavier_tank_escarcha/sounds/snowfall.mp3"),MECH_MAX_DISTANCE_SOUND, this);
    _sound_engine->mediaPlayer()->playlist()->setPlaybackMode(QMediaPlaylist::Loop);

    _pixmaps = getListOfPixmapFromStripImage(":xavier_tank_escarcha/sprites/skill_2.png", 300);

    _aura = getListOfPixmapFromStripImage(":/xavier_tank_escarcha/sprites/aura.png", 90);

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
                                  "<td><p class=\"w\" align=\"justify\">Ralentización: </p></td>"
                                  "<td><p class=\"yl\" align=\"justify\">%1 %</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de effecto: </p></td>"
                                  "<td><p class=\"yl\" align=\"justify\">%2</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Costo de mana: </p></td>"
                                  "<td><p class=\"bl\" align=\"justify\">%3</p></td>"
                                  "</tr>"
                                  "<tr>"
                                  "<td><p class=\"w\" align=\"justify\">Tiempo de recarga: </p></td>"
                                  "<td><p class=\"gll\" align=\"justify\">%4 seg</p></td>"
                                  "</tr>"
                                  "</table>").arg(_slow_down_percent).arg(_time_effect).arg(_mana_cost).arg((double)_cooldown/1000,0,'f',1);

        description.append(details);
    }

    return description;
}

void Skill2Factory::updateAttributes()
{
    switch (getLevel())
    {
        case 2:
            _slow_down_percent = 20;
            _mana_cost = 26;
            _time_effect = 6;
            _cooldown = 20000;
        break;
        case 3:
            _slow_down_percent = 30;
            _mana_cost = 30;
            _time_effect = 7;
            _cooldown = 25000;
        break;
        case 4:
            _slow_down_percent = 40;
            _mana_cost = 34;
            _time_effect = 8;
            _cooldown = 30000;
        break;
    }
}

ISkill *Skill2Factory::createSkill()
{
    ISkill* skill = new Skill2(_tank, _slow_down_percent, _time_effect, _pixmaps, _aura, _sound_engine);
    skill->setZValue(_tank->zValue()+0.1);
    skill->setPos(_tank->pos());

    _sound_engine->stopSound();
    _sound_engine->playSound2D(_tank->pos(),_tank->principalReference()->pos(), Configuration::volumeSound());

    return skill;
}
