#include "drains.h"
#include "itank.h"

Drains::Drains(ITank* tank, int mana, QList<QPixmap> pixmaps, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    _tank = tank;

    _mana = mana;

    _speed = 5;

    _pixmaps = pixmaps;

    _positions.append(scenePos());
}

QRectF Drains::boundingRect() const
{
    return QRectF(-15, -15, 15, 15);
}

QPainterPath Drains::shape() const
{
    QPainterPath s;
    s.addRect(-5, -5, 5, 5);
    return s;
}

void Drains::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    for(int i = _positions.length() - 1; i >= 0; --i)
    {
        QPointF position = _positions.at(i);
        painter->drawPixmap(mapFromScene(position.x()-5,position.y()-5), _pixmaps.at(i));
    }
}

void Drains::advance(int phase)
{
    if(!phase)
        return;

    if(_tank->getStatus() == DEAD)
    {
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    if(_positions.size() == 6)
        _positions.removeAt(_positions.size()-1);
    _positions.insert(0, scenePos());

    QLineF line = QLineF(scenePos(), _tank->scenePos());
    qreal angle = ((360 - ((int)line.angle()))+90)%360;
    double angle_rad = angle * PIx2 / 360;
    double cosangle = cos( angle_rad );
    double sinangle = sin( angle_rad );

    setPos(x()-sinangle*-_speed, y()+cosangle*-_speed);

    if(collidesWithItem((QGraphicsItem*)_tank))
    {
        _tank->setCurrentMana(_tank->currentMana() + _mana);
        scene()->removeItem(this);
        deleteLater();
    }
}
