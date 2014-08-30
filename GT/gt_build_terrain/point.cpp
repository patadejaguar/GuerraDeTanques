#include "point.h"

Point::Point(qreal x, qreal y, QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    setPos(x, y);
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QColor(0, 162, 232));
    painter->setBrush(QBrush(QColor(153, 217, 234)));
    painter->drawEllipse(-2.5, -2.5, 5, 5);
}

QRectF Point::boundingRect() const
{
    return QRectF(-2.5, -2.5, 5, 5);
}

