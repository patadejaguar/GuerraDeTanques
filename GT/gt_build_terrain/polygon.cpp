#include "polygon.h"

Polygon::Polygon(QList<Point*> points, QGraphicsScene *scene, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    _points = points;
    scene->addItem(this);
    foreach (Point* point, _points) {
        scene->addItem(point);
    }
}

void Polygon::removePoint(Point *point)
{
    scene()->removeItem(point);
    _points.removeAll(point);
    if(_points.size() < 3)
        deleteLater();
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPolygonF polygon;
    foreach (Point* point, _points) {
        polygon.append(point->pos());
    }
    painter->setBrush(QBrush(QColor(153, 217, 234, 80)));
    painter->drawPolygon(polygon);
}

QRectF Polygon::boundingRect() const
{
    QPolygonF polygon;
    foreach (Point* point, _points) {
        polygon.append(point->pos());
    }
    return polygon.boundingRect();
}
