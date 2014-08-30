#include "effect.h"
#include "itank.h"

Effect::Effect(ITank* creator_tank, ITank* target_tank, double bonus_live_regeneration, double bonus_mana_regeneration, double bonus_armor, double bonus_speed, bool stunned, bool p_forever, int time, QList<QPixmap> sprite, qreal index_z_value):
    QGraphicsObject()
{
    _creator_tank = creator_tank;
    _target_tank = target_tank;

    connect(_target_tank, &ITank::notifyPosition, this, &Effect::setPosition);
    connect(_target_tank, SIGNAL(notifyDead(ITank*)), this, SLOT(deleteLater()));

    setPosition(_target_tank->pos());
    setZValue(_target_tank->zValue()+index_z_value);

    _bonus_live_regeneration = bonus_live_regeneration;
    _bonus_mana_regeneration = bonus_mana_regeneration;
    _bonus_armor = bonus_armor;
    _bonus_speed = bonus_speed;
    _stunned = stunned;

    _forever = p_forever;
    _time = time;

    _sprite = sprite;
    _step = 0;
}

ITank *Effect::creatorTank()
{
    return _creator_tank;
}

ITank *Effect::targetTank()
{
    return _target_tank;
}

bool Effect::isForever()
{
    return _forever;
}

int Effect::time()
{
    return _time;
}

bool Effect::nextTime()
{
    return --_time > 0;
}

void Effect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(_sprite.isEmpty())
        return;

    QPixmap pixmap = _sprite.at(_step);
    if(!pixmap.isNull())
        painter->drawPixmap(-pixmap.width()/2, -pixmap.height()/2, pixmap.width(), pixmap.height(), pixmap);
}

void Effect::advance(int phase)
{
    if(!phase)
        return;

    if(++_step >= _sprite.size())
        _step = 0;
}

void Effect::destroy()
{
    scene()->removeItem(this);
    deleteLater();
}

bool Effect::stunned() const
{
    return _stunned;
}

void Effect::setStunned(bool stunned)
{
    _stunned = stunned;
}


QRectF Effect::boundingRect() const
{
    return _sprite.isEmpty() ? QRectF() : _sprite.at(0).rect().translated(-_sprite.at(0).width()/2, -_sprite.at(0).height()/2);
}

void Effect::setPosition(QPointF pos)
{
    QGraphicsObject::setPos(pos);
}

double Effect::bonusLiveRegeneration() const
{
    return _bonus_live_regeneration;
}

void Effect::setBonusLiveRegeneration(int bonus_live_regeneration)
{
    _bonus_live_regeneration = bonus_live_regeneration;
}

double Effect::bonusManaRegeneration() const
{
    return _bonus_mana_regeneration;
}

void Effect::setBonusManaRegeneration(int bonus_mana_regeneration)
{
    _bonus_mana_regeneration = bonus_mana_regeneration;
}

double Effect::bonusArmor() const
{
    return _bonus_armor;
}

void Effect::setBonusArmor(int bonus_armor)
{
    _bonus_armor = bonus_armor;
}

double Effect::bonusSpeed() const
{
    return _bonus_speed;
}

void Effect::setBonusSpeed(int bonus_speed)
{
    _bonus_speed = bonus_speed;
}
