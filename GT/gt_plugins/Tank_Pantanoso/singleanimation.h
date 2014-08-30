#ifndef SINGLE_ANIMATION_H
#define SINGLE_ANIMATION_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "utilities.h"

class SingleAnimation : public QObject, public QGraphicsPixmapItem
{
        Q_OBJECT

    public:

        explicit SingleAnimation(QPointF pos, int angle, QList<QPixmap> pixmaps, QObject *parent = 0);

        ~SingleAnimation();

    protected:

        void advance(int phase);

    private:

        int _step;

        QList<QPixmap> _pixmaps;
};

#endif // SINGLE_ANIMATION_H
