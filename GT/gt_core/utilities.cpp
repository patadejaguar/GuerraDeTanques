#include "utilities.h"

QList<QPixmap> getListOfPixmapFromStripImage(QString image, int w)
{
    QPixmap base(image);

    QList<QPixmap> pixmaps;

    int h = base.height();
    int cant = base.width()/w;
    for(int i = 0; i< cant ; ++i)
        pixmaps.append(base.copy(i*w,0,w,h));

    return pixmaps;
}

QList<QPixmap> getListOfPixmapFromTileSet(QPixmap tile_set, int w)
{
    QList<QPixmap> pixmaps;

    int h = tile_set.height();
    int cant = tile_set.width()/w;
    for(int i = 0; i< cant ; ++i)
        pixmaps.append(tile_set.copy(i*w,0,w,h));

    return pixmaps;
}

QPixmap transformQPixmapToGrayScale(QPixmap pixmap){
    QImage image = pixmap.toImage();
    for(int i = 0; i < image.width(); ++i)
        for(int j = 0; j < image.height(); ++j)
        {
            QColor base = image.pixel(i, j);
            int value = (base.red()+base.green()+base.blue())/3;
            image.setPixel(i, j, QColor(value, value, value, base.alpha()).rgba());
        }
    return QPixmap::fromImage(image);
}

int getPart(int total, int percent)
{
    return (total * percent) / 100;
}

int getPercent(int total, int percent)
{
    return (percent * 100) / total;
}

double getDistance(QPointF p1, QPointF p2)
{
    return pow(pow(p1.x()-p2.x(),2)+pow(p1.y()-p2.y(),2),0.5);
}
