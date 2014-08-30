#include "singleanimation.h"

SingleAnimation::SingleAnimation(QPointF pos, int angle, QList<QPixmap> pixmaps, QObject *parent) :
    QObject(parent),
    QGraphicsPixmapItem()
{
    _pixmaps = pixmaps;

    setPixmap(_pixmaps.at(0));

    setPos(pos.x()-15,pos.y()-49);

    setTransformOriginPoint(mapFromScene(pos));

    setRotation(angle);

    _step = -1;
}

SingleAnimation::~SingleAnimation()
{
    scene()->removeItem(this);
}

void SingleAnimation::advance(int phase)
{
    if(!phase)
        return;

    if(++_step < _pixmaps.size())
        setPixmap(_pixmaps.at(_step));
    else
        deleteLater();
}
