#include "configuration.h"

//observers
QList<ConfigurationObserver*> Configuration::_observers = QList<ConfigurationObserver*>();
//controls
int Configuration::_key_forward = Qt::Key_W;
int Configuration::_key_backward = Qt::Key_S;
int Configuration::_key_turn_left = Qt::Key_A;
int Configuration::_key_turn_right = Qt::Key_D;
int Configuration::_key_skill_1 = Qt::LeftButton;
int Configuration::_key_skill_2 = Qt::Key_Space;
int Configuration::_key_skill_3 = Qt::RightButton;
int Configuration::_key_update_1 = Qt::Key_1;
int Configuration::_key_update_2 = Qt::Key_2;
int Configuration::_key_update_3 = Qt::Key_3;
//sound
int Configuration::_volume_music = 50;
int Configuration::_volume_sound = 50;
//network
QString Configuration::_network_server = "127.0.0.1";
qint16 Configuration::_network_server_port = 9090;
bool Configuration::_network_use_ssl = false;
//logs
bool Configuration::_activate_logs = false;

void Configuration::addObserver(ConfigurationObserver *observer)
{
    Configuration::_observers.append(observer);
}

void Configuration::removeObserver(ConfigurationObserver *observer)
{
    Configuration::_observers.removeAll(observer);
}

void Configuration::save()
{
    QSettings settings("game_config", QSettings::IniFormat);
    //controls
    settings.setValue("key_forward", Configuration::_key_forward);
    settings.setValue("key_backward", Configuration::_key_backward);
    settings.setValue("key_turn_left", Configuration::_key_turn_left);
    settings.setValue("key_turn_right", Configuration::_key_turn_right);
    settings.setValue("key_skill_1", Configuration::_key_skill_1);
    settings.setValue("key_skill_2", Configuration::_key_skill_2);
    settings.setValue("key_skill_3", Configuration::_key_skill_3);
    settings.setValue("key_update_1", Configuration::_key_update_1);
    settings.setValue("key_update_2", Configuration::_key_update_2);
    settings.setValue("key_update_3", Configuration::_key_update_3);
    //sound
    settings.setValue("volume_music", Configuration::_volume_music);
    settings.setValue("volume_sound", Configuration::_volume_sound);
    //network
    settings.setValue("network_server", Configuration::_network_server);
    settings.setValue("network_server_port", Configuration::_network_server_port);
    settings.setValue("network_use_ssl", Configuration::_network_use_ssl);
    //logs
    settings.setValue("activate_logs", Configuration::_activate_logs);
}

void Configuration::load()
{
    QSettings settings("game_config", QSettings::IniFormat);
    //controls
    Configuration::_key_forward = settings.value("key_forward", 0x057).toInt();
    Configuration::_key_backward = settings.value("key_backward", 0x053).toInt();
    Configuration::_key_turn_left = settings.value("key_turn_left", 0x041).toInt();
    Configuration::_key_turn_right = settings.value("key_turn_right", 0x044).toInt();
    Configuration::_key_skill_1 = settings.value("key_skill_1", 0x00000001).toInt();
    Configuration::_key_skill_2 = settings.value("key_skill_2", 0x20).toInt();
    Configuration::_key_skill_3 = settings.value("key_skill_3", 0x00000002).toInt();
    Configuration::_key_update_1 = settings.value("key_update_1", 0x31).toInt();
    Configuration::_key_update_2 = settings.value("key_update_2", 0x32).toInt();
    Configuration::_key_update_3 = settings.value("key_update_3", 0x33).toInt();
    //sound
    Configuration::_volume_music = settings.value("volume_music", 50).toInt();
    Configuration::_volume_sound = settings.value("volume_sound", 50).toInt();
    //network
    Configuration::_network_server = settings.value("network_server", "127.0.0.1").toString();
    Configuration::_network_server_port = settings.value("network_server_port", 9090).toInt();
    Configuration::_network_use_ssl = settings.value("network_use_ssl", false).toBool();
    //logs
    Configuration::_activate_logs = settings.value("activate_logs", false).toBool();

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        observer->update();
}

void Configuration::restore()
{
    //controls
    Configuration::_key_forward = Qt::Key_W;
    Configuration::_key_backward = Qt::Key_S;
    Configuration::_key_turn_left = Qt::Key_A;
    Configuration::_key_turn_right = Qt::Key_D;
    Configuration::_key_skill_1 = Qt::LeftButton;
    Configuration::_key_skill_2 = Qt::Key_Space;
    Configuration::_key_skill_3 = Qt::RightButton;
    Configuration::_key_update_1 = Qt::Key_1;
    Configuration::_key_update_2 = Qt::Key_2;
    Configuration::_key_update_3 = Qt::Key_3;
    //sound
    Configuration::_volume_music = 50;
    Configuration::_volume_sound = 50;
    //network
    Configuration::_network_server = "127.0.0.1";
    Configuration::_network_server_port = 9090;
    Configuration::_network_use_ssl = false;
    //logs
    Configuration::_activate_logs = false;
    //save
    Configuration::save();

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        observer->update();
}

QList<int> Configuration::controls()
{
    QList<int> controls;
    controls.append(Configuration::_key_forward);
    controls.append(Configuration::_key_backward);
    controls.append(Configuration::_key_turn_left);
    controls.append(Configuration::_key_turn_right);
    controls.append(Configuration::_key_skill_1);
    controls.append(Configuration::_key_skill_2);
    controls.append(Configuration::_key_skill_3);
    controls.append(Configuration::_key_update_1);
    controls.append(Configuration::_key_update_2);
    controls.append(Configuration::_key_update_3);
    return controls;
}

void Configuration::setControls(QList<int> controls)
{
    Configuration::_key_forward = controls.at(0);
    Configuration::_key_backward = controls.at(1);
    Configuration::_key_turn_left = controls.at(2);
    Configuration::_key_turn_right = controls.at(3);
    Configuration::_key_skill_1 = controls.at(4);
    Configuration::_key_skill_2 = controls.at(5);
    Configuration::_key_skill_3 = controls.at(6);
    Configuration::_key_update_1 = controls.at(7);
    Configuration::_key_update_2 = controls.at(8);
    Configuration::_key_update_3 = controls.at(9);

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_CONTROLS))
            observer->update();
}

QList<QString> Configuration::controlsName()
{
    QList<int> controls = Configuration::controls();
    QList<QString> controls_name;
    foreach(int key, controls)
    {
        if(key == Qt::NoButton)
            controls_name.append("");
        else if(key == Qt::LeftButton)
            controls_name.append("Click Izq");
        else if(key == Qt::RightButton)
            controls_name.append("Click Der");
        else if(key == Qt::MiddleButton)
            controls_name.append("Click Med");
        else
            controls_name.append(QKeySequence(key).toString());
    }
    return controls_name;
}

int Configuration::volumeMusic()
{
    return Configuration::_volume_music;
}

void Configuration::setVolumeMusic(int volume_music)
{
    Configuration::_volume_music = volume_music;

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_SOUND))
            observer->update();
}

int Configuration::volumeSound()
{
    return Configuration::_volume_sound;
}

void Configuration::setVolumeSound(int volume_sound)
{
    Configuration::_volume_sound = volume_sound;

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_SOUND))
            observer->update();
}

qint16 Configuration::networkServerPort()
{
    return Configuration::_network_server_port;
}

void Configuration::setNetworkServerPort(const qint16 &network_server_port)
{
    Configuration::_network_server_port = network_server_port;

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_NETWORK))
            observer->update();
}

QString Configuration::networkServer()
{
    return Configuration::_network_server;
}

void Configuration::setNetworkServer(const QString &network_server)
{
    Configuration::_network_server = network_server;

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_NETWORK))
            observer->update();
}

bool Configuration::networkUseSsl()
{
    return Configuration::_network_use_ssl;
}

void Configuration::setNetworkUseSsl(bool use_ssl)
{
    Configuration::_network_use_ssl = use_ssl;

    foreach (ConfigurationObserver* observer, Configuration::_observers)
        if(observer->isTypeOf(COT_NETWORK))
            observer->update();
}
bool Configuration::activateLogs()
{
    return _activate_logs;
}

void Configuration::setActivateLogs(bool activate_logs)
{
    _activate_logs = activate_logs;
}


ConfigurationObserver::ConfigurationObserver(QList<ConfigurationObserverType> types)
{
    _types = types;
}

QList<ConfigurationObserverType> ConfigurationObserver::types() const
{
    return _types;
}

bool ConfigurationObserver::isTypeOf(ConfigurationObserverType type)
{
    return _types.contains(type);
}
