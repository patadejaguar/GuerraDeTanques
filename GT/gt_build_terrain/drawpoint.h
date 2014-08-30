#ifndef DRAWPOINT_H
#define DRAWPOINT_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDataStream>

#include "tile.h"

enum Status {RELEASE, HOVER, PRESS};

class DrawPoint : public QGraphicsItem
{

    public:
        DrawPoint();
        DrawPoint(Tile *up_left, Tile *up_right, Tile *down_left, Tile *down_right);
        DrawPoint(int x, int y, Tile *up_left, Tile *up_right, Tile *down_left, Tile *down_right, int layers);
        ~DrawPoint();
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        QRectF boundingRect() const;
        void setStatus(Status value);
        inline Tile* getDownRightTile(){return _down_right;}
        inline int getX() const {return _x;}
        inline void setX(int value) {_x = value;}
        inline int getY() const {return _y;}
        inline void setY(int value) {_y = value;}
        inline QList<bool> getLayersActivate() const {return _layers_activate;}
        inline void setLayersActivate(QList<bool> value) {_layers_activate = value;}

    private:
        Status _status;
        int _x, _y;
        Tile *_up_left;
        Tile *_up_right;
        Tile *_down_left;
        Tile *_down_right;
        QList<bool> _layers_activate;
};

QDataStream &operator<< (QDataStream &stream, const DrawPoint &draw_point);
QDataStream &operator>> (QDataStream &stream, DrawPoint &draw_point);

#endif // DRAWPOINT_H
