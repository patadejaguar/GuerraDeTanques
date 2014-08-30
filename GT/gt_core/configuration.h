#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSize>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include <QKeySequence>

#include "gt_core_global.h"
#include "itank.h"

enum ConfigurationObserverType {COT_CONTROLS, COT_SOUND, COT_NETWORK};

class GT_CORESHARED_EXPORT ConfigurationObserver
{
    public:

        ConfigurationObserver(QList<ConfigurationObserverType> types);

        QList<ConfigurationObserverType> types() const;

        bool isTypeOf(ConfigurationObserverType type);

        virtual void update() = 0;

    private:

        QList<ConfigurationObserverType> _types;

};

class GT_CORESHARED_EXPORT Configuration: public QObject
{
        Q_OBJECT

    public:

        static void                             addObserver(ConfigurationObserver* observer);

        static void                             removeObserver(ConfigurationObserver* observer);

        static void                             save();

        static void                             load();

        static void                             restore();

        static QList<int>                       controls();
        static void                             setControls(QList<int> controls);

        static QList<QString>                   controlsName();

        static int                              volumeMusic();
        static void                             setVolumeMusic(int volume_music);

        static int                              volumeSound();
        static void                             setVolumeSound(int volume_sound);

        static QString                          networkServer();
        static void                             setNetworkServer(const QString &network_server);

        static qint16                           networkServerPort();
        static void                             setNetworkServerPort(const qint16 &network_server_port);

        static bool                             networkUseSsl();
        static void                             setNetworkUseSsl(bool use_ssl);

        static bool                             activateLogs();
        static void                             setActivateLogs(bool activate_logs);

    private:
        //observers
        static QList<ConfigurationObserver*>    _observers;

        //controls
        static int                              _key_forward;
        static int                              _key_backward;
        static int                              _key_turn_left;
        static int                              _key_turn_right;
        static int                              _key_skill_1;
        static int                              _key_skill_2;
        static int                              _key_skill_3;
        static int                              _key_update_1;
        static int                              _key_update_2;
        static int                              _key_update_3;

        //sound
        static int                               _volume_music;
        static int                               _volume_sound;

        //network
        static QString                          _network_server;
        static qint16                           _network_server_port;
        static bool                             _network_use_ssl;

        //log
        static bool                             _activate_logs;

};

#endif // CONFIGURATION_H
