#ifndef GRID_H
#define GRID_H

#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>

#include "iobserver.h"
#include "iobservable.h"

class Grid : public QGraphicsItem, public IObserver
{

public:
    explicit Grid(QGraphicsItem *parent = 0);
    virtual void update(IObservable *observable);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

signals:

public slots:

protected:
};

#endif // GRID_H
