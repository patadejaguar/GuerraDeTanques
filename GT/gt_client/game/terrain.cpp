#include "terrain.h"
#include "resourcemanager.h"

Terrain* Terrain::_instance = 0;

Terrain::Terrain(QObject* parent):
    QObject(parent)
{

}

QList<QGraphicsPixmapItem *> Terrain::map() const
{
    return _map;
}

QPixmap Terrain::miniMapView() const
{
    return QPixmap::fromImage(_mini_map_view);
}

int Terrain::counterPlayersGreen() const
{
    return _counter_players_green;
}

int Terrain::counterPlayersRed() const
{
    return _counter_players_red;
}

Terrain *Terrain::instance()
{
    if(!_instance)
        return _instance = new Terrain();
    return _instance;
}

void Terrain::load(QString file_name)
{
    QFile file(DIR_TERRAINS.absoluteFilePath(file_name));

    if(file.open(QIODevice::ReadOnly))
    {
        emit notifyLoadSteps(QString("Cargando parámetros principales."));
        qApp->processEvents();

        QDataStream stream(&file);
        //propiedades del mapa
        stream>>_version;
        if(_version > GAME_VERSION)
        {
            emit notifyError(QString("Versión incompatible del terreno."));
            file.close();
            return;
        }
        stream>>_name;
        stream>>_counter_players_green;
        stream>>_counter_players_red;
        stream>>_image;
        stream>>_tile_width;
        stream>>_tile_height;
        stream>>_width;
        stream>>_height;

        emit notifyLoadSteps(QString("Verificando dependencias de objetos."));
        qApp->processEvents();
        //dependencias de los plugins de objetos
        QStringList dependencies;
        stream >> dependencies;

        QList<IObjectFactory*> object_factories = ResourceManager::instance()->objectFactories();
        foreach (IObjectFactory* factory, object_factories)
            dependencies.removeAll(factory->nameForDependency());

        if(!dependencies.isEmpty())
        {
            QString d;
            foreach(QString dep, dependencies)
                d+=dep+"\n";
            emit notifyError(QString("No se encuentran los ficheros:%1").arg(d));
            file.close();
            return;
        }

        emit notifyLoadSteps(QString("Cargando objetos."));
        qApp->processEvents();
        qDeleteAll(_objects);
        _objects.clear();

        //objetos
        int size_objects;
        stream >> size_objects;
        while(size_objects--)
        {
            QString dependency;
            stream >> dependency;
            foreach (IObjectFactory* factory, object_factories)
            {
                if(factory->nameForDependency() == dependency)
                {
                    IObject* object = factory->createInstance();
                    object->loadFromData(stream);
                    _objects.append(object);
                }
            }
        }

        emit notifyLoadSteps(QString("Cargando texturas."));
        qApp->processEvents();
        _tile_sets.clear();
        //cargando conjuntos de tiles
        int tiles_sets_count;
        stream >> tiles_sets_count;

        for(int i = 0 ; i < tiles_sets_count ; ++i )
        {
            TileSet tile_set;

            stream >> tile_set._tiles;
            stream >> tile_set._mini_map_tiles;

            _tile_sets.append(tile_set);
        }

        emit notifyLoadSteps(QString("Construyendo el terreno."));
        qApp->processEvents();
        qDeleteAll(_map);
        _map.clear();

        //cargando el mapa y el mini mapa
        _mini_map_view = QImage(_tile_width*_width, _tile_height*_height, QImage::Format_ARGB32_Premultiplied);
        _mini_map_view.fill(Qt::transparent);
        QPainter painter_for_mini_map(&_mini_map_view);

        for (int i = 0; i < _width; ++i)
        {
            for(int j = 0 ;j < _height; ++j)
            {
                int x, y;
                QList<int> tile_number_layers;

                stream >> x >> y;
                stream >> tile_number_layers;

                QImage image (_tile_width, _tile_height, QImage::Format_ARGB32_Premultiplied);                
                QPainter painter(&image);

                for(int k = tile_number_layers.size()-1; k >= 0 ; --k)
                {
                    int tile_number = tile_number_layers.at(k);
                    if(tile_number > 0)
                    {
                        QPixmap tile = _tile_sets.at(k)._tiles.at(tile_number-1);
                        QPixmap mini_map_tile = _tile_sets.at(k)._mini_map_tiles.at((tile_number-1)>14?14:tile_number-1);

                        painter.drawPixmap(0, 0, _tile_width, _tile_height, tile);
                        painter_for_mini_map.drawPixmap(x, y, _tile_width, _tile_height, mini_map_tile);
                    }
                }
                QGraphicsPixmapItem *tile = new QGraphicsPixmapItem(QPixmap::fromImage(image));
                tile->setPos(x, y);
                tile->setZValue(-1000);
                _map.append(tile);
            }
        }

        emit notifyLoadSteps(QString("Cargando obstrucciones."));
        qApp->processEvents();
        _obstructed_areas.clear();
        stream >> _obstructed_areas;

        file.close();

        emit notifyLoadFinished();
    }
    else
    {
        emit notifyError(QString("El terreno no puede ser leido."));
        return;
    }
}
