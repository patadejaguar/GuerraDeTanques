#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>

#include "terrain.h"
#include "itank.h"
#include "iobject.h"

class ResourceManager: public QObject
{

        Q_OBJECT

    public:

        static ResourceManager*     instance();

        QStringList                 terrainList() const;
        QStringList                 objectList() const;
        QStringList                 tankList() const;

        QStringList                 replayList() const;

        QList<TerrainInfo>          terrainsInfo() const;
        QList<ITankFactory*>        tankFactories() const;
        QList<IObjectFactory*>      objectFactories() const;

    public slots:

        void                        connectedData(QStringList terrains, QStringList tanks, QStringList objects);
        void                        resourcesAreReady();

        void                        updateLocalResources();

    signals:

        void                        notifyResourceAreReady();

    private:

        explicit ResourceManager();

        void                        updateTerrainListInfo();
        void                        updateObjectListInfo();
        void                        updateTankListInfo();


        static ResourceManager*     _instance;

        QList<TerrainInfo>          _terrains_info;
        QList<ITankFactory*>        _tank_factories;
        QList<IObjectFactory*>      _object_factories;

        QStringList                 _terrain_list;
        QStringList                 _object_list;
        QStringList                 _tank_list;

};

#endif // RESOURCEMANAGER_H
