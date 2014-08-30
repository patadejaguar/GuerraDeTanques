#ifndef SERVERCONFIGURATION_H
#define SERVERCONFIGURATION_H

#include <QObject>
#include <QString>
#include <QList>
#include <QSize>
#include "databaseconnection.h"

class ServerConfiguration: public QObject
{
        Q_OBJECT

    public:

        static ServerConfiguration* instance();

        bool useSsl() const;
        void setUseSsl(bool useSsl);

        qint16 getPort() const;
        void setPort(const qint16 &getPort);

        static DataBaseConnection* getNewDataBaseConnection();

    signals:

    private:

        ServerConfiguration(QObject *parent = 0);

        static ServerConfiguration* _instance;
        bool                        _use_ssl;
        qint16                      _port;

};

#endif // SERVERCONFIGURATION_H
