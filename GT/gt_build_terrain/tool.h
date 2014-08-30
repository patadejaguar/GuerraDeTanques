#ifndef TOOL_H
#define TOOL_H

#include <QPixmap>
#include <QList>
#include <QDataStream>
#include <QBuffer>

class Tool
{
    public:
        Tool();
        Tool(QString name, bool obstruction, QPixmap image, QColor mini_map_color);
        ~Tool();

        int getVariationsSize();
        inline QString name() const {return _name;}
        inline void setName(QString name) {_name = name;}
        inline QList<QPixmap> tiles() const {return _tiles;}
        inline void setTiles(QList<QPixmap> tiles) {_tiles = tiles;}
        inline QList<QPixmap> tilesForMiniMap() const {return _tiles_for_mini_map;}
        inline void setTilesForMiniMap(QList<QPixmap> tiles_for_mini_map) {_tiles_for_mini_map = tiles_for_mini_map;}
        inline QColor miniMapColor() const {return _mini_map_color;}
        inline void setMiniMapColor(QColor mini_map_color) {_mini_map_color = mini_map_color;}
        inline QPixmap getTile(int value){return _tiles.at(value);}
        inline QPixmap getTileForMiniMap(int value){if(value>14) value = 14; return _tiles_for_mini_map.at(value);}
        void generate(QDataStream &stream);

        bool obstruction() const;
        void setObstruction(bool obstruction);

    private:
        QString _name;
        QList<QPixmap> _tiles;
        QList<QPixmap> _tiles_for_mini_map;
        QColor _mini_map_color;
        bool  _obstruction;
};

QDataStream &operator<< (QDataStream &stream, const Tool &tool);
QDataStream &operator>> (QDataStream &stream, Tool &tool);

#endif // TOOL_H
