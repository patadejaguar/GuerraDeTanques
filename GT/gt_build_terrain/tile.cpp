#include "tile.h"
#include "terrain.h"

Tile::Tile()
{
    _image = QImage(Terrain::instance()->tileWidth(), Terrain::instance()->tileHeight(), QImage::Format_ARGB32_Premultiplied);
    _image_for_mini_map = QImage(Terrain::instance()->tileWidth(), Terrain::instance()->tileHeight(), QImage::Format_ARGB32_Premultiplied);
}

Tile::Tile(int x, int y, int width, int height, int layers) :
    QGraphicsItem()
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    for(int i = 0; i < layers ; ++i)
        _tile_number_layers.append(0);
    _image = QImage(_width, _height, QImage::Format_ARGB32_Premultiplied);
    _image_for_mini_map = QImage(_width, _height, QImage::Format_ARGB32_Premultiplied);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

//    for (int i = _tile_number_layers.size()-1; i >= 0 ; --i)
//    {
//        int tile_number = _tile_number_layers.at(i);
//        if(tile_number > 0)
//        {
//            const qreal zoom = option->levelOfDetailFromTransform(painter->worldTransform());
//            if(zoom <= 0.4)
//            {
//                Tool *tool = Terrain::instance()->terrainTools().at(i);
//                QPixmap pixmap = tool->getTileForMiniMap(tile_number-1);
//                painter->drawPixmap(_x, _y, _width, _height, pixmap);
//            }
//            else
//            {
//                Tool *tool = Terrain::instance()->terrainTools().at(i);
//                alpha = tool->tileHaveAlpha(tile_number-1);
//                QPixmap pixmap = tool->getTile(tile_number-1);
//                painter->drawPixmap(_x, _y, _width, _height, pixmap);
//            }
//        }
//    }
    const qreal zoom = option->levelOfDetailFromTransform(painter->worldTransform());
    if(zoom <= 0.4)
    {
        painter->drawImage(_x, _y, _image_for_mini_map);
    }
    else
    {
        painter->drawImage(_x, _y, _image);
    }
}

QRectF Tile::boundingRect() const
{
    return QRectF(_x, _y, _width, _height);
}

void Tile::setTileNumber(int layer, int value)
{
    _tile_number_layers[layer] = value;
    addVariation(layer);    

    updateImages();
}

void Tile::addVariation(int layer)
{
    Terrain *terrain = Terrain::instance();
    if(terrain->isDrawRandom())
    {
        if(_tile_number_layers[layer] >= 15)
        {
            int max_variation = terrain->activateTool()->getVariationsSize();
            int variation = 0;
            if(max_variation != 0 && qrand()%7 == 1)
                variation = qrand()%(max_variation + 1);
            _tile_number_layers[layer] = 15 + variation;
            updateImages();
        }
    }
    else
    {
        if(_tile_number_layers[layer] >= 15)
        {
            _tile_number_layers[layer] = 15 + terrain->activateRandom();
            updateImages();
        }
    }
    update();
}

QList<bool> Tile::getLayersActivated()
{
    QList<bool> activated_layers;
    for(int i = 0; i<_tile_number_layers.size(); ++i)
        activated_layers.append(_tile_number_layers.at(1)!=0);
    return activated_layers;
}

void Tile::generate(QDataStream &stream, QPainter &painter_obstruction)
{
    stream << _x;
    stream << _y;
    stream << _tile_number_layers;

    QList<Tool*> tools = Terrain::instance()->terrainTools();

    for(int i = 0; i<_tile_number_layers.size();++i)
    {
        int tile_number = _tile_number_layers.at(i);
        if(tile_number > 0)
            if(tools.at(i)->obstruction())
                painter_obstruction.drawPixmap(_x,_y,_width,_height,tools.at(i)->getTile(tile_number-1));
    }
}

void Tile::updateImages()
{
    _image.fill(Qt::transparent);
    _image_for_mini_map.fill(Qt::transparent);

    QPainter painter(&_image);
    QPainter painter_for_mini_map(&_image_for_mini_map);
    for (int i = _tile_number_layers.size()-1; i >= 0 ; --i)
    {
        int tile_number = _tile_number_layers.at(i);
        if(tile_number > 0)
        {
            Tool *tool = Terrain::instance()->terrainTools().at(i);
            painter.drawPixmap(0, 0, _width, _height, tool->getTile(tile_number-1));
            painter_for_mini_map.drawPixmap(0, 0, _width, _height, tool->getTileForMiniMap(tile_number-1));
        }
    }
}


QDataStream &operator <<(QDataStream &stream, const Tile &tile)
{
    stream << tile.x();
    stream << tile.y();
    stream << tile.width();
    stream << tile.height();
    stream << tile.tile_number_layers();

    return stream;
}

QDataStream &operator >>(QDataStream &stream, Tile &tile)
{
    int x,y,w,h;
    QList<int> t;

    stream >> x;
    stream >> y;
    stream >> w;
    stream >> h;
    stream >> t;

    tile.setX(x);
    tile.setY(y);
    tile.setWidth(w);
    tile.setHeight(h);
    tile.setTile_number_layers(t);

    tile.updateImages();

    return stream;
}
