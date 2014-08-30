#include "drawpoint.h"
#include "terrain.h"

DrawPoint::DrawPoint():QGraphicsItem()
{}

DrawPoint::DrawPoint(Tile *up_left, Tile *up_right, Tile *down_left, Tile *down_right):
    QGraphicsItem()
{
    _status = RELEASE;
    setAcceptHoverEvents(true);
    _up_left = up_left;
    _up_right = up_right;
    _down_left = down_left;
    _down_right = down_right;
}

DrawPoint::DrawPoint(int x, int y, Tile *up_left, Tile *up_right, Tile *down_left, Tile *down_right, int layers):
    QGraphicsItem()
{
    _status = RELEASE;
    setAcceptHoverEvents(true);
    _x = x;
    _y = y;
    _up_left = up_left;
    _up_right = up_right;
    _down_left = down_left;
    _down_right = down_right;
    for(int i = 0; i < layers - 1 ; ++i)
        _layers_activate.append(false);
    _layers_activate.append(true);
}
DrawPoint::~DrawPoint()
{

}

void DrawPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(_status == RELEASE)
        return;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if(_status == HOVER)
        brush.setColor(QColor(255,255,255,50));
    else if(_status == PRESS)
        brush.setColor(QColor(255,255,255,60));

    Terrain *terrain = Terrain::instance();
    int t_w = terrain->tileWidth();
    int t_h = terrain->tileHeight();

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(_x-t_w/2,_y-t_h/2,t_w,t_h);
}

QRectF DrawPoint::boundingRect() const
{
    Terrain *terrain = Terrain::instance();
    int t_w = terrain->tileWidth();
    int t_h = terrain->tileHeight();
    return QRectF(_x-t_w/2,_y-t_h/2,t_w+3,t_h+3);
}

void DrawPoint::setStatus(Status value)
{
    _status = value;
    int layer =Terrain::instance()->activateToolPos();

    if(_status == PRESS)
    {
        for(int i = 0; i < layer ; ++i)
        {
            if(_layers_activate[i])
            {
                int value = 0;
                if(_up_left != NULL && _up_left->getTileNumber(i) >= 1)
                {
                    value = _up_left->getTileNumber(i)-1;
                    if(_up_left->getTileNumber(i)>15)
                        value -= _up_left->getTileNumber(i)-15;
                    _up_left->setTileNumber(i, value);
                }
                if(_up_right != NULL && _up_right->getTileNumber(i) >= 2)
                {
                    value = _up_right->getTileNumber(i)-2;
                    if(_up_right->getTileNumber(i)>15)
                        value -= _up_right->getTileNumber(i)-15;
                    _up_right->setTileNumber(i, value);
                }
                if(_down_left != NULL && _down_left->getTileNumber(i) >= 4)
                {
                    value = _down_left->getTileNumber(i)-4;
                    if(_down_left->getTileNumber(i)>15)
                        value -= _down_left->getTileNumber(i)-15;
                    _down_left->setTileNumber(i, value);
                }
                if(_down_right != NULL && _down_right->getTileNumber(i) >= 8)
                {
                    value = _down_right->getTileNumber(i)-8;
                    if(_down_right->getTileNumber(i)>15)
                        value -= _down_right->getTileNumber(i)-15;
                    _down_right->setTileNumber(i, value);
                }
                _layers_activate[i] = false;
            }
        }
    }
    if(_status == PRESS && !_layers_activate.at(layer))
    {
        if(_up_left != NULL)
            _up_left->setTileNumber(layer, _up_left->getTileNumber(layer)+1);
        if(_up_right != NULL)
            _up_right->setTileNumber(layer, _up_right->getTileNumber(layer)+2);
        if(_down_left != NULL)
            _down_left->setTileNumber(layer, _down_left->getTileNumber(layer)+4);
        if(_down_right != NULL)
            _down_right->setTileNumber(layer, _down_right->getTileNumber(layer)+8);

        _layers_activate[layer] = true;
    }
    else if(_status == PRESS)
    {
//        if(_up_left != NULL)
//            _up_left->addVariation(layer);
//        if(_up_right != NULL)
//            _up_right->addVariation(layer);
//        if(_down_left != NULL)
//            _down_left->addVariation(layer);
        if(_down_right != NULL)
            _down_right->addVariation(layer);
    }
}

QDataStream &operator<<(QDataStream &stream, const DrawPoint &draw_point)
{
    stream << draw_point.getX();
    stream << draw_point.getY();
    stream << draw_point.getLayersActivate();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DrawPoint &draw_point)
{
    int x,y;
    QList<bool> layers_activate;
    stream >> x;
    stream >> y;
    stream >> layers_activate;
    draw_point.setX(x);
    draw_point.setY(y);
    draw_point.setLayersActivate(layers_activate);
    return stream;
}
