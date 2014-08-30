#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>

#include "iobservable.h"
#include "tool.h"
#include "drawpoint.h"

#include "polygon.h"
#include "iobject.h"
#include "treearea.h"
#include "circleofpower.h"

enum LayerType {L_TERRAIN, L_OBJECT, L_OBSTRUCTIONS};

class Terrain : public IObservable
{
        Q_OBJECT

    public:

        static Terrain *instance();

        void reset();

        void newTerrain();

        void generate(QDataStream &stream);

        inline int version() const {return _version;}
        inline void setVersion(int version){_version = version;}

        inline QString name() const {return _name;}
        inline void setName(QString value){_name = value;}

        inline void setCountersPlayers(int green, int red){_counter_green = green; _counter_red = red;}

        inline int tileWidth() const {return _tile_width;}
        inline void setTileWidth(int value){_tile_width = value;}
        inline int tileHeight() const {return _tile_height;}
        inline void setTileHeight(int value){_tile_height = value;}
        inline int width() const {return _width;}
        inline int height() const {return _height;}
        inline int widthInPx() const{return _width*_tile_width;}
        inline int heightInPx() const{return _height*_tile_height;}

        inline QStringList dependencies() const
        {
            QStringList dep;
            foreach (IObject *object, _objects)
                if(!dep.contains(object->nameForDependency()))
                    dep<<object->nameForDependency();

            return dep;
        }
        inline QStringList objectFactoriesNameForDependency() const
        {
            QStringList dep;
            foreach (IObjectFactory *factory, _object_factories)
                if(!dep.contains(factory->pluginName()))
                    dep << factory->nameForDependency();

            return dep;
        }

        void setProperties(QString name, int width, int height);
        void setProperties(QString name, int width, int height, int tile_width, int tile_height);

        QList< QList<DrawPoint*>* > drawPoints() const {return _draw_points;}
        QList< QList<Tile*>* > tiles() const {return _tiles;}
        void loadTilesAndDrawPoints(QDataStream &stream);

        inline LayerType currentLayer(){return _current_layer;}
        inline void setCurrentLayer(LayerType value){_current_layer = value;}

        inline QList<Tool*> terrainTools() const {return _terrain_tools;}
        bool parseTools(QXmlStreamReader &stream);
        bool parseTool(QXmlStreamReader &stream);
        void loadTerrainTools(QDataStream &stream);
        void loadObjectFactories();

        inline void setActivateTool(int value){_active_tool = value;}
        inline void setDrawRandom(bool value){_draw_random = value;}
        inline bool isDrawRandom(){return _draw_random;}
        inline void setActivateRandom(int value){_active_random = value;}
        inline int activateRandom(){return _active_random;}
        inline int activateToolPos(){return _active_tool;}
        inline Tool* activateTool(){return _terrain_tools.at(_active_tool);}
        inline int cursorSize(){return _cursor_size;}
        inline void setCursorSize(int value){_cursor_size = value;}

        inline QList<IObjectFactory*> objectFactories() const{return _object_factories;}
        inline void setActivateObjectFactory(int value){_active_object_factory = value;}
        inline IObjectFactory* activateObjectFactory(){return _object_factories.at(_active_object_factory);}

        inline QList<IObject*> objects() const{return _objects;}
        inline void addObject(IObject* object){_objects.append(object);}
        inline void removeObject(IObject *object){_objects.removeAll(object);}

        QImage image() const;
        void setImage(const QImage &image);

    signals:

    public slots:

    private:

        explicit Terrain();
        static Terrain *_instance;

        int _version;
        QString _name;
        int _tile_width, _tile_height, _width, _height;

        int _counter_green, _counter_red;

        LayerType _current_layer;

        QList< QList<DrawPoint*>* > _draw_points;
        QList< QList<Tile*>* > _tiles;

        QList<Tool*> _terrain_tools;
        bool _draw_random;
        int _active_tool;
        int _active_random;
        int _cursor_size;

        QList<IObjectFactory*> _object_factories;
        QList<IObject*> _objects;
        int _active_object_factory;

        QImage _image;
        QImage* _image_obstruction;
        QList< QList<bool> > _border;
        QPolygon _polygon;
        bool isCorner(int i, int j);
        void findNeighborsInLine(int &i, int &j, int dir_i, int dir_j);


};

QDataStream &operator<<(QDataStream &stream, const Terrain &terrain);
QDataStream &operator>>(QDataStream &stream, Terrain &terrain);

#endif // TERRAIN_H
