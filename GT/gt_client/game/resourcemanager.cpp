#include "resourcemanager.h"
#include "connectiontcp.h"

ResourceManager* ResourceManager::_instance = 0;

ResourceManager::ResourceManager()
{
    QDir app_dir = QDir(QCoreApplication::applicationDirPath());
    app_dir.mkdir("terrains");
    app_dir.mkdir("tanks");
    app_dir.mkdir("objects");
    app_dir.mkdir("replays");

    connect(ConnectionTcp::instance(),&ConnectionTcp::notifyConnectedData, this, &ResourceManager::connectedData);
    connect(ConnectionTcp::instance(),&ConnectionTcp::notifyResourcesAreReady, this, &ResourceManager::resourcesAreReady);

    updateLocalResources();
}

QStringList ResourceManager::tankList() const
{
    return _tank_list;
}

QStringList ResourceManager::replayList() const
{
    return DIR_REPLAYS.entryList(QStringList()<<"*.rpl", QDir::Files, QDir::Name);
}

void ResourceManager::updateTankListInfo()
{
    qDeleteAll(_tank_factories);
    _tank_factories.clear();

    QStringList files = DIR_TANKS.entryList(_tank_list, QDir::Files, QDir::Name);
    foreach (QString file_name, files)
    {
        QPluginLoader plugin_loader(DIR_TANKS.absoluteFilePath(file_name));
        QObject *tank_plugin = plugin_loader.instance();

        if(tank_plugin)
        {
            if(tank_plugin->inherits("ITankFactory"))
            {
                ITankFactory *tank_factory = qobject_cast<ITankFactory *>(tank_plugin);
                if(tank_factory->gameVersion() == GAME_VERSION)
                {
                    _tank_factories.append(tank_factory);
                }
            }
        }
    }
}

QStringList ResourceManager::objectList() const
{
    return _object_list;
}

void ResourceManager::updateObjectListInfo()
{
    qDeleteAll(_object_factories);
    _object_factories.clear();

    IObjectFactory* circle_of_power = new CircleOfPowerFactory();
    _object_factories.append(circle_of_power);

    QStringList files = DIR_OBJECTS.entryList(_object_list, QDir::Files, QDir::Name);
    foreach(QString file, files)
    {
        QPluginLoader plugin_loader(DIR_OBJECTS.absoluteFilePath(file));
        QObject *object_plugin = plugin_loader.instance();

        if(object_plugin)
        {
            if(object_plugin->inherits("IObjectFactory"))
            {
                IObjectFactory* object_factory = qobject_cast<IObjectFactory *>(object_plugin);
                if(object_factory->gameVersion() == GAME_VERSION)
                {
                    _object_factories.append(object_factory);
                }
            }
        }
    }
}

QStringList ResourceManager::terrainList() const
{
    return _terrain_list;
}

void ResourceManager::updateTerrainListInfo()
{
    _terrains_info.clear();

    QDir dir_terrains = DIR_TERRAINS;
    QStringList files = dir_terrains.entryList(_terrain_list, QDir::Files,QDir::Name);
    foreach (QString file_name, files)
    {
        QFile file(DIR_TERRAINS.absoluteFilePath(file_name));
        if(!file.open(QIODevice::ReadOnly))
            return;
        QDataStream stream(&file);
        int version;
        stream>>version;
        if(version == GAME_VERSION)
        {
            TerrainInfo terrain_info;
            stream>>terrain_info._name;
            stream>>terrain_info._green_count;
            stream>>terrain_info._red_count;
            stream>>terrain_info._image;
            _terrains_info.append(terrain_info);
        }
    }
}

QList<TerrainInfo> ResourceManager::terrainsInfo() const
{
    return _terrains_info;
}

QList<ITankFactory *> ResourceManager::tankFactories() const
{
    return _tank_factories;
}

QList<IObjectFactory *> ResourceManager::objectFactories() const
{
    return _object_factories;
}

void ResourceManager::connectedData(QStringList terrains, QStringList tanks, QStringList objects)
{
    _terrain_list = terrains;
    _tank_list = tanks;
    _object_list = objects;

    QStringList local_terrains = DIR_TERRAINS.entryList(QStringList()<<"*.gmm", QDir::Files);
    QStringList local_tanks = DIR_TANKS.entryList(QStringList()<<"*.dll", QDir::Files);
    QStringList local_objects = DIR_OBJECTS.entryList(QStringList()<<"*.dll", QDir::Files);

    QStringList terrains_missing;
    QStringList tanks_missing;
    QStringList objects_missing;

    foreach(QString terrain, terrains)
        if(!local_terrains.contains(terrain))
            terrains_missing.append(terrain);

    foreach(QString tank, tanks)
        if(!local_tanks.contains(tank))
            tanks_missing.append(tank);

    foreach(QString object, objects)
        if(!local_objects.contains(object))
            objects_missing.append(object);

    if(terrains_missing.isEmpty() && tanks_missing.isEmpty() && objects_missing.isEmpty())
        resourcesAreReady();
    else
        ConnectionTcp::instance()->sendMissingsResources(terrains_missing, tanks_missing, objects_missing);
}

void ResourceManager::resourcesAreReady()
{
    updateTerrainListInfo();
    updateTankListInfo();
    updateObjectListInfo();

    emit notifyResourceAreReady();
}

void ResourceManager::updateLocalResources()
{
    _terrain_list   = DIR_TERRAINS.entryList(QStringList()<<"*.gmm", QDir::Files);
    _tank_list      = DIR_TANKS.entryList(QStringList()<<"*.dll", QDir::Files);
    _object_list    = DIR_OBJECTS.entryList(QStringList()<<"*.dll", QDir::Files);

    updateTerrainListInfo();
    updateTankListInfo();
    updateObjectListInfo();
}

ResourceManager *ResourceManager::instance()
{
    if(!_instance)
        _instance = new ResourceManager();
    return _instance;
}
