#ifndef DRAINS_H
#define DRAINS_H

#include <QGraphicsObject>
class ITank;

class Drains : public QGraphicsObject
{
        Q_OBJECT

    public:

        explicit Drains(ITank* tank, int mana, QList<QPixmap> pixmaps, QGraphicsItem *parent = 0);

        QRectF                  boundingRect() const;

        QPainterPath            shape() const;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    public slots:

        void                    advance(int phase);

    private:

        ITank*                  _tank;

        int                     _mana;

        QList<QPixmap>          _pixmaps;

        QList<QPointF>          _positions;

        int                     _speed;

};

#endif // DRAINS_H
