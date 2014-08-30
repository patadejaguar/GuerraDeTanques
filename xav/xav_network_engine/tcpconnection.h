#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QSslSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QMutex>

#include <QSslCipher>

#include "xav_network_engine_global.h"

const int TCP_SIZE_OF_DATA = 10;

class XAV_NETWORK_ENGINESHARED_EXPORT TcpConnection: public QThread
{
        Q_OBJECT

    public:

        explicit            TcpConnection(bool use_ssl, int socket_descriptor, QObject *parent=0);

        explicit            TcpConnection(bool use_ssl, QObject *parent=0);

        explicit            TcpConnection(int socket_descriptor, QObject *parent=0);

        QString             address();

        qint16              getPort();

    public slots:

        void                start();

        void                stop();

    signals:

        void                notifyConnected();

        void                notifyDisconnected();

    protected:

        virtual void        run() = 0;

        void                sendData(QByteArray data);

        virtual void        parseData(QDataStream &stream_input) = 0;

        //------------------_att...

        bool                _running;

        bool                _use_ssl;

        int                 _socket_descriptor;

        QString             _address;

        qint16              _port;

        QList<QByteArray>   _buffer_data_to_write;

        QMutex              _mutex;
};

#endif // TCPCONNECTION_H
