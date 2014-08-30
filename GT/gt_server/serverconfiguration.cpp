#include "serverconfiguration.h"

ServerConfiguration * ServerConfiguration::_instance = NULL;

ServerConfiguration *ServerConfiguration::instance()
{
    if(_instance == NULL)
        _instance = new ServerConfiguration();
    return _instance;
}

ServerConfiguration::ServerConfiguration(QObject *parent):QObject(parent)
{
    _port = 9090;
}

bool ServerConfiguration::useSsl() const
{
    return _use_ssl;
}

void ServerConfiguration::setUseSsl(bool use_ssl)
{
    _use_ssl = use_ssl;
}

qint16 ServerConfiguration::getPort() const
{
    return _port;
}

void ServerConfiguration::setPort(const qint16 &port)
{
    _port = port;
    if(_port<0)
        _port = 0;
}

DataBaseConnection *ServerConfiguration::getNewDataBaseConnection()
{
    return new DataBaseConnection("gt.sqlite","QSQLITE");
}

