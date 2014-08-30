#include "grid.h"
#include "terrain.h"

Grid::Grid(QGraphicsItem *parent) :
    QGraphicsItem(parent), IObserver()
{
}

void Grid::update(IObservable *observable)
{
    QString name = observable->metaObject()->className() ;
    if(name == "Terrain")
        QGraphicsItem::update();
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    Terrain *terrain = Terrain::instance();
    int t_w = terrain->tileWidth();
    int t_h = terrain->tileHeight();
    int w = terrain->width();
    int h = terrain->height();

    if(w <= 0 || h <= 0 || t_w <= 0 || t_h <= 0 )
        return;

    QPen pen;
    pen.setWidthF(0.3);
    pen.setColor(QColor(250,250,250));
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    for (int i = 0; i <= w*t_w; i+=t_w)
    {
        painter->setPen(pen);
        QPainterPath line;
        line.moveTo(i,0);
        line.lineTo(i,h*t_h);
        painter->drawPath(line);
    }
    for (int i = 0; i <= h*t_h; i+=t_h)
    {
        painter->setPen(pen);
        QPainterPath line;
        line.moveTo(0,i);
        line.lineTo(w*t_w,i);
        painter->drawPath(line);
    }
}

QRectF Grid::boundingRect() const
{
    Terrain *property_terrain = Terrain::instance();
    int t_w = property_terrain->tileWidth();
    int t_h = property_terrain->tileHeight();
    int w = property_terrain->width();
    int h = property_terrain->height();
    return QRectF(0,0,w*t_w,h*t_h);
}
