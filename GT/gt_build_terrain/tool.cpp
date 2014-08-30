#include "tool.h"
#include "terrain.h"
#include "utilities.h"

Tool::Tool()
{
}

Tool::Tool(QString name, bool obstruction, QPixmap pixmap, QColor mini_map_color)
{
    _name = name;
    _obstruction = obstruction;
    _mini_map_color = mini_map_color;

    int tile_width = Terrain::instance()->tileWidth();
    int tile_height = Terrain::instance()->tileHeight();
    int image_width = pixmap.width();

    QImage aux = pixmap.toImage();
    QImage mask = QImage(tile_width*15, tile_height, QImage::Format_ARGB32_Premultiplied);
    mask.fill(Qt::transparent);
    for(int i = 0; i< mask.width(); ++i )
        for(int j = 0; j< mask.height(); ++j )
            if(qAlpha(aux.pixel(i,j)) != 0)
                mask.setPixel(i, j, _mini_map_color.rgba());

    _tiles_for_mini_map = getListOfPixmapFromTileSet(QPixmap::fromImage(mask), tile_width);

    for(int i = 0; i < image_width/tile_width; ++i )
    {
        _tiles.append(pixmap.copy(i*tile_width,0,tile_width,tile_height));
    }

}

Tool::~Tool()
{
}

int Tool::getVariationsSize()
{
    int tile_width = Terrain::instance()->tileWidth();
    return _tiles.size()*tile_width/tile_width-15;
}

void Tool::generate(QDataStream &stream)
{
//    stream << _mini_map_color;
    stream << _tiles;
    stream << _tiles_for_mini_map;
//    stream << _tiles.size();
//    foreach (QPixmap tile, _tiles)
//    {
//        QByteArray data;
//        QBuffer buffer(&data);
//        tile.save(&buffer, "PNG", 100);
//        stream << data;
//    }
}
bool Tool::obstruction() const
{
    return _obstruction;
}

void Tool::setObstruction(bool obstruction)
{
    _obstruction = obstruction;
}


QDataStream &operator <<(QDataStream &stream, const Tool &tool)
{
    stream << tool.name();
    stream << tool.obstruction();
    stream << tool.miniMapColor();
    stream << tool.tiles();
    stream << tool.tilesForMiniMap();

    return stream;
}

QDataStream &operator >>(QDataStream &stream, Tool &tool)
{
    QString name;
    bool obstruction;
    QColor mini_map_color;
    QList<QPixmap> tiles;
    QList<QPixmap> tiles_for_mini_map;

    stream >> name;
    stream >> obstruction;
    stream >> mini_map_color;
    stream >> tiles;
    stream >> tiles_for_mini_map;

    tool.setName(name);
    tool.setObstruction(obstruction);
    tool.setMiniMapColor(mini_map_color);
    tool.setTiles(tiles);
    tool.setTilesForMiniMap(tiles_for_mini_map);

    return stream;
}
