#ifndef POLYGON_H
#define POLYGON_H

#include <QGraphicsObject>
#include <QGraphicsScene>

#include "point.h"

class Polygon : public QGraphicsObject
{
    public:
        explicit Polygon(QList<Point *> points, QGraphicsScene* scene, QGraphicsItem *parent = 0);
        void    removePoint(Point* point);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;

    private:
        QList<Point*> _points;

};

#endif // POLYGON_H
