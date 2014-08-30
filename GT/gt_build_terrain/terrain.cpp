#include "terrain.h"

Terrain* Terrain::_instance = 0;

Terrain::Terrain() :
    IObservable()
{
    _version = 1;
    _tile_width = _tile_height = _width = _height = _active_tool = _active_random = 0;
    _cursor_size = 1;
    _draw_random = true;
    _current_layer = L_TERRAIN;

    _counter_green = _counter_red = 0;

}
QImage Terrain::image() const
{
    return _image;
}

void Terrain::setImage(const QImage &image)
{
    _image = image;
}


bool Terrain::isCorner(int i, int j)
{
    if(i >= 0 && i < _image_obstruction->width() && j >= 0 && j < _image_obstruction->height() && _border[i][j])
        return true;
    return false;
}

void Terrain::findNeighborsInLine(int &i, int &j, int dir_i, int dir_j)
{
    j += dir_j;
    i += dir_i;
    while(isCorner(i, j))
    {
        _border[i][j] = false;
        j += dir_j;
        i += dir_i;
    }

    j -= dir_j;
    i -= dir_i;

    _polygon.append(QPoint(i,j));
}

Terrain *Terrain::instance()
{
    if(!_instance)
        _instance = new Terrain();
    return _instance;
}

void Terrain::setProperties(QString name, int width, int height)
{
    _name = name;
    _width = width;
    _height = height;
    notifyObservers();
}

void Terrain::setProperties(QString name, int width, int height, int tile_width, int tile_height)
{
    _tile_width = tile_width;
    _tile_height = tile_height;
    setProperties(name, width, height);
}

void Terrain::loadTilesAndDrawPoints(QDataStream &stream)
{
    for (int i = 0; i <= _width; ++i)
    {
        QList<DrawPoint*> *row_draw_point = new QList<DrawPoint*>();
        _draw_points.append(row_draw_point);

        QList<Tile*> *row_tile = new QList<Tile*>();
        if(i < _width)
            _tiles.append(row_tile);
        else
            delete row_tile;

        for (int j = 0; j <= _height; ++j)
        {
            Tile *up_left = NULL;
            Tile *up_right = NULL;
            Tile *down_left = NULL;
            Tile *down_right = NULL;

            if((i-1 >= 0 && j-1 >= 0 ) && (i <= _width && j <= _height))
                up_left = _tiles.at(i-1)->at(j-1);
            if((i >= 0 && j-1 >= 0) && (i < _width && j <= _height))
                up_right = _tiles.at(i)->at(j-1);
            if((i-1 >= 0 && j >= 0 ) && (i <= _width && j < _height))
                down_left = _tiles.at(i-1)->at(j);
            if(i < _width && j < _height)
            {
                down_right = new Tile();
                stream >> *down_right;
                down_right->setZValue(0);
                row_tile->append(down_right);
            }
            DrawPoint *draw_point = new DrawPoint(up_left, up_right, down_left, down_right);
            stream >> *draw_point;
            draw_point->setZValue(2);
            row_draw_point->append(draw_point);
        }
    }

}

bool Terrain::parseTools(QXmlStreamReader &stream)
{
    stream.readNext();
    stream.readNext();
    if(stream.name() == "tools")
    {
        QXmlStreamAttributes attributes = stream.attributes();
        _tile_width = attributes.value("tile_width").toString().toInt();
        _tile_height = attributes.value("tile_height").toString().toInt();

        while(!stream.atEnd())
        {
            if(!parseTool(stream))
                return false;
        }
    }
    else
    {
        QMessageBox::critical(0,"Error","Este no es un fichero de herramientas.");
        return false;
    }
    if (stream.hasError())
    {
        QMessageBox::critical(0,"Error",stream.errorString());
        return false;
    }
    return true;
}

bool Terrain::parseTool(QXmlStreamReader &stream)
{
    while(stream.readNext() != QXmlStreamReader::StartElement)
    {
        if(stream.atEnd())
            return true;
    }
    if(stream.name() == "tool")
    {
        QXmlStreamAttributes attributes = stream.attributes();
        QString name = attributes.value("name").toString();
        bool obstruction = attributes.value("obstruction").toInt();
        QColor mini_map_color = QColor(attributes.value("mini_map_color").toString());
        if(name == "")
        {
            QMessageBox::critical(0,"Error",QString("Error en la linea %1.\nNo se puede leer el atributo name.").arg(stream.lineNumber()));
            return  false;
        }
        QPixmap image;
        if(!image.loadFromData(qUncompress(QByteArray::fromBase64(attributes.value("image").toUtf8()))))
        {
            QMessageBox::critical(0,"Error",QString("Error en la linea %1.\nNo se puede leer el atributo image.").arg(stream.lineNumber()));
            return  false;
        }
        _terrain_tools.append(new Tool(name, obstruction, image, mini_map_color));
        return true;
    }
    QMessageBox::critical(0,"Error",QString("Error en la linea %1.\nNo se reconoce como una herramienta.").arg(stream.lineNumber()));
    return false;
}

void Terrain::loadTerrainTools(QDataStream &stream)
{
    int size;
    stream >> size;
    for(int i = 0; i < size; ++i)
    {
        Tool *tool = new Tool();
        stream >> *tool;
        _terrain_tools.append(tool);
    }
}

void Terrain::loadObjectFactories()
{
    IObjectFactory* circle_of_power = new CircleOfPowerFactory();
    _object_factories.append(circle_of_power);

    QDir plugins_dir(qApp->applicationDirPath());
    plugins_dir.cd("objects");
    QStringList files = plugins_dir.entryList(QStringList()<<"*.dll", QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);

    foreach(QString file, files)
    {
        QPluginLoader plugin_loader(plugins_dir.absoluteFilePath(file));
        QObject *object_plugin = plugin_loader.instance();
        if(object_plugin)
            if(object_plugin->inherits("IObjectFactory"))
                _object_factories.append(qobject_cast<IObjectFactory *>(object_plugin));
    }
}

void Terrain::reset()
{
    removeAllObservers();
    _name = "";
    _tile_width = _tile_height = _width = _height = _active_tool = _active_random  = 0;
    _cursor_size = 1;
    _draw_random = true;

    qDeleteAll(_terrain_tools);
    _terrain_tools.clear();

    qDeleteAll(_draw_points);
    _draw_points.clear();

    qDeleteAll(_tiles);
    _tiles.clear();

    qDeleteAll(_objects);
    _objects.clear();
}

void Terrain::newTerrain()
{
    for (int i = 0; i <= _width; ++i)
    {
        QList<DrawPoint*> *row_draw_point = new QList<DrawPoint*>();
        _draw_points.append(row_draw_point);

        QList<Tile*> *row_tile = new QList<Tile*>();
        _tiles.append(row_tile);

        for (int j = 0; j <= _height; ++j)
        {
            Tile *up_left = NULL;
            Tile *up_right = NULL;
            Tile *down_left = NULL;
            Tile *down_right = NULL;

            if((i-1 >= 0 && j-1 >= 0 ) && (i <= _width && j <= _height))
                up_left = _tiles.at(i-1)->at(j-1);
            if((i >= 0 && j-1 >= 0) && (i < _width && j <= _height))
                up_right = _tiles.at(i)->at(j-1);
            if((i-1 >= 0 && j >= 0 ) && (i <= _width && j < _height))
                down_left = _tiles.at(i-1)->at(j);
            if(i < _width && j < _height)
            {
                down_right = new Tile(i*_tile_width,j*_tile_height,_tile_width,_tile_height,_terrain_tools.size());
                down_right->setTileNumber(_terrain_tools.size()-1,15);
                down_right->setZValue(0);
                row_tile->append(down_right);
            }
            DrawPoint *draw_point = new DrawPoint(i*_tile_width, j*_tile_height, up_left, up_right, down_left, down_right,_terrain_tools.size());
            draw_point->setZValue(2);
            row_draw_point->append(draw_point);
        }
    }
}

void Terrain::generate(QDataStream &stream)
{
    //propiedades del mapa
    stream << _version;                                          //versión
    stream << _name;

    stream << _counter_green << _counter_red;

    QByteArray data_image;
    QBuffer buffer(&data_image);
    //_image = _image.scaled(1024,768,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    _image.save(&buffer,"JPG",100);

    stream << data_image;
    stream << _tile_width;
    stream << _tile_height;
    stream << _width;
    stream << _height;

    //dependencias de los plugins de objetos
    stream << dependencies();

    //objetos
    stream << _objects.size();
    foreach (IObject* object, _objects)
       object->save(stream);

    //guardando las herramientas de terreno
    stream << _terrain_tools.size();
    foreach (Tool *tool, _terrain_tools)
        tool->generate(stream);

    //guardando el mapa y las obstrucciones
        _image_obstruction = new QImage(widthInPx(),heightInPx(),QImage::Format_ARGB32_Premultiplied);
        _image_obstruction->fill(Qt::transparent);
        QPainter painter_obstruction(_image_obstruction);

        for (int i = 0; i < _width; ++i)
            for(int j = 0 ;j < _height; ++j)
                    _tiles.at(i)->at(j)->generate(stream, painter_obstruction);

        painter_obstruction.end();
        _border.clear();
        QImage *im = new QImage(widthInPx(),heightInPx(),QImage::Format_ARGB32_Premultiplied);
        im->fill(Qt::transparent);
        for(int i = 0; i < widthInPx(); i++)
        {
            _border.append(QList<bool>());
            for(int j = 0; j < heightInPx(); j++)
            {
                bool corner = false;
                bool fill = qAlpha(_image_obstruction->pixel(i,j)) != 0;
                if(fill)
                {
                    if((i > 0 && qAlpha(_image_obstruction->pixel(i-1,j)) == 0) || i == 0)
                        corner = true;
                    if((!corner && i < widthInPx()-1 && qAlpha(_image_obstruction->pixel(i+1,j)) == 0) || i == widthInPx()-1)
                        corner = true;

                    if((!corner && j > 0 && qAlpha(_image_obstruction->pixel(i,j-1)) == 0) || j == 0)
                        corner = true;
                    if((!corner && j < heightInPx()-1 && qAlpha(_image_obstruction->pixel(i,j+1)) == 0) || j == heightInPx()-1)
                        corner = true;
                }
                _border[i].append(fill && corner);
                if(fill && corner)
                    im->setPixel(i,j,qRgb(255,0,0));
            }
        }
        //im->save("border.png","PNG",100);
        delete im;

        QList<TreeArea*> tree_areas;
        for(int i = 0; i < widthInPx(); i++)
        {
            for(int j = 0; j < heightInPx(); j++)
            {
                int x = i;
                int y = j;
                while((!_polygon.isEmpty() && _polygon.last() != QPoint(x,y)) || _border[i][j])
                {
                    if(isCorner(i, j))
                    {
                        _border[i][j] = false;
                        _polygon.append(QPoint(i,j));
                    }

                    if(isCorner(i, j+1))
                        findNeighborsInLine(i, j, 0, +1);

                    else if(isCorner(i, j-1))
                        findNeighborsInLine(i, j, 0, -1);

                    else if(isCorner(i+1, j))
                        findNeighborsInLine(i, j, +1, 0);

                    else if(isCorner(i-1, j))
                        findNeighborsInLine(i, j, -1, 0);

                    else if(isCorner(i+1, j+1))
                        findNeighborsInLine(i, j, 1, 1);

                    else if(isCorner(i-1, j-1))
                        findNeighborsInLine(i, j, -1, -1);

                    else if(isCorner(i-1, j+1))
                        findNeighborsInLine(i, j, -1, +1);

                    else if(isCorner(i+1, j-1))
                        findNeighborsInLine(i, j, +1, -1);

                    else if(!isCorner(i, j))
                        break;
                }
                if(!_polygon.isEmpty())
                {
                    bool no_inside = true;
                    for(int i = 0; i<tree_areas.size() && no_inside; ++i)
                    {
                        if(tree_areas.at(i)->value().containsPoint(_polygon.at(0), Qt::WindingFill))
                        {
                            tree_areas.at(i)->addChildren(new TreeArea(_polygon));
                            no_inside = false;
                        }
                    }
                    if(no_inside)
                        tree_areas.append(new TreeArea(_polygon));

                    _polygon.clear();
                }
            }
        }

        delete _image_obstruction;

        QList<QPainterPath> areas;
        foreach (TreeArea *area, tree_areas)
            areas.append(area->getAreas());

        QImage *im_borders = new QImage(widthInPx(),heightInPx(),QImage::Format_ARGB32_Premultiplied);
        im_borders->fill(Qt::transparent);
        QPainter painter_border(im_borders);
        foreach (QPainterPath area, areas)
            painter_border.drawPath(area);
        //im_borders->save("obstructions.png","PNG",100);

        stream << areas;
}

QDataStream &operator<<(QDataStream &stream, const Terrain &terrain)
{
    //propiedades del mapa
    stream << terrain.version();
    stream << terrain.name();
    stream << terrain.tileWidth();
    stream << terrain.tileHeight();
    stream << terrain.width();
    stream << terrain.height();

    //dependencias de los plugins de objetos
    stream<<terrain.dependencies();

    //objetos
    stream << terrain.objects().size();
    foreach (IObject* object, terrain.objects())
       object->save(stream);

    //herramientas
    stream << terrain.terrainTools().size();
    foreach (Tool *tool, terrain.terrainTools())
        stream << *tool;

    //tiles y draw_points
    for (int i = 0; i <= terrain.width(); ++i)
    {
        for(int j = 0 ;j <= terrain.height(); ++j)
        {
            if(i < terrain.width() && j < terrain.height())
            {
                stream << *terrain.tiles().at(i)->at(j);
            }
            stream << *terrain.drawPoints().at(i)->at(j);
        }
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Terrain &terrain)
{
    //propiedades del terreno.
    QString name;
    int v, t_w, t_h, w, h;
    stream >> v;
    stream >> name;
    stream >> t_w;
    stream >> t_h;
    stream >> w;
    stream >> h;
    terrain.setVersion(v);
    terrain.setProperties(name,w, h, t_w, t_h);

    //dependencias de los plugins de objetos
    QStringList dependencies;
    stream >> dependencies;

    foreach (QString factory_name, terrain.objectFactoriesNameForDependency())
        dependencies.removeAll(factory_name);

    if(!dependencies.isEmpty())
    {
        QString d;
        foreach(QString dep, dependencies)
            d+=dep+"\n";
        QMessageBox::critical(0,"Error",QString("Dependencias no resueltas: %1").arg(d));
    }

    //objetos
    int size_objects;
    stream >> size_objects;
    while(size_objects--)
    {
        QString dependency;
        stream >> dependency;
        foreach (IObjectFactory* factory, terrain.objectFactories())
        {
            if(factory->nameForDependency() == dependency)
            {
                IObject* object = factory->createInstance(true);
                object->loadFromData(stream);
                terrain.addObject(object);
            }
        }
    }

    //herramientas
    terrain.loadTerrainTools(stream);

    //tiles y draw_points
    terrain.loadTilesAndDrawPoints(stream);

    return stream;
}
