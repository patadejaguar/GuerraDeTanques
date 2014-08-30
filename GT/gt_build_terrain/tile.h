#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDataStream>
#include <QStyleOptionGraphicsItem>

class Tile :public QGraphicsItem
{

    public:

        Tile();

        explicit Tile(int x, int y, int width, int height, int layers);

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        QRectF boundingRect() const;

        inline int getTileNumber(int layer){return _tile_number_layers.at(layer);}

        QList<bool> getLayersActivated();

        void setTileNumber(int layer, int value);

        void addVariation(int layer);

        void generate(QDataStream &stream, QPainter &painter_obstruction);

        inline int x() const{return _x;}
        inline void setX(int x){_x = x;}

        inline int y() const{return _y;}
        inline void setY(int y){_y = y;}

        inline int width() const{return _width;}
        inline void setWidth(int width){_width = width;}

        inline int height() const{return _height;}
        inline void setHeight(int height){_height = height;}

        QList<int> tile_number_layers() const{return _tile_number_layers;}
        void setTile_number_layers(const QList<int> &tile_number_layers){_tile_number_layers = tile_number_layers;}

        void updateImages();

    private:        
        int _x, _y, _width, _height;
        QList<int> _tile_number_layers;
        QImage  _image;
        QImage  _image_for_mini_map;
};

QDataStream &operator<< (QDataStream &stream, const Tile &tile);
QDataStream &operator>> (QDataStream &stream, Tile &tile);

#endif // TILE_H
