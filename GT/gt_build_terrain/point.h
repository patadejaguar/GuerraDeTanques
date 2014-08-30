#ifndef POINT_H
#define POINT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>

class Point : public QGraphicsItem
{
    public:

        explicit Point(qreal x, qreal y, QGraphicsItem *parent = 0);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;

};

#endif // POINT_H
