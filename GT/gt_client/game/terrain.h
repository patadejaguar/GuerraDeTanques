#ifndef TERRAIN_H
#define TERRAIN_H

#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QBuffer>
#include <QBitmap>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>

#include "iobject.h"
#include "circleofpower.h"
#include "gameglobals.h"

struct Tile
{
        int             _x;
        int             _y;
        QList<int>      _tile_number_layers;
        QPixmap         _pixmap;
};

struct TileSet
{
        QList<QPixmap>  _tiles;
        QList<QPixmap>  _mini_map_tiles;
};

struct TerrainInfo
{
        QString         _name;
        int             _green_count;
        int             _red_count;
        QPixmap         _image;
};
Q_DECLARE_METATYPE(TerrainInfo)
inline QDataStream &operator<<(QDataStream &stream, const TerrainInfo &ti)
{
    stream << ti._name << ti._image << ti._green_count << ti._red_count;
    return stream;
}
inline QDataStream &operator>>(QDataStream &stream, TerrainInfo &ti)
{
    stream >> ti._name >> ti._image >> ti._green_count >> ti._red_count;
    return stream;
}

class Terrain: public QObject
{
        Q_OBJECT

    public:
        static                      Terrain *instance();

        inline int                  getWidthInTile(){return _width;}
        inline int                  getHeightInTile(){return _height;}
        inline int                  getTileWidth()const {return _tile_width;}
        inline int                  getTileHeight()const {return _tile_height;}
        inline int                  getWidthInPX() const {return _width * _tile_width;}
        inline int                  getHeightInPX() const {return _height * _tile_height;}

        inline QList<QPainterPath>  getObstructedAreas(){return _obstructed_areas;}
        inline QList<IObject*>      objects() const {return _objects;}

        int                         counterPlayersGreen() const;
        int                         counterPlayersRed() const;

        QList<QGraphicsPixmapItem *> map() const;

        QPixmap                      miniMapView() const;

    public slots:
        void                        load(QString file_name);

    signals:
        void                        notifyLoadSteps(QString step);
        void                        notifyError(QString error);
        void                        notifyLoadFinished();

    private:
        explicit Terrain(QObject *parent = 0);
        static Terrain*             _instance;
        int                         _version;
        QString                     _name;
        int                         _counter_players_green;
        int                         _counter_players_red;
        QByteArray                  _image;
        int                         _tile_width;
        int                         _tile_height;
        int                         _width;
        int                         _height;
        QList<TileSet>              _tile_sets;
        QList<QGraphicsPixmapItem*> _map;
        QImage                      _mini_map_view;
        QList<QPainterPath>         _obstructed_areas;

        QList<IObject*>             _objects;
};

#endif // TERRAIN_H
