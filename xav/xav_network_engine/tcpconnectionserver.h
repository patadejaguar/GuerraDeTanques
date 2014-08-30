#ifndef TCPCONNECTIONSERVER_H
#define TCPCONNECTIONSERVER_H

#include "tcpconnection.h"

#include <QTcpSocket>
//#include <QSslSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QDataStream>

#include "xav_network_engine_global.h"

class XAV_NETWORK_ENGINESHARED_EXPORT TcpConnectionServer: public TcpConnection
{
        Q_OBJECT

    public:

        explicit TcpConnectionServer( bool use_ssl = false, int socket_descriptor = -1, QObject *parent=0);

    protected:

        virtual void        parseData(QDataStream &stream_input) = 0;

        void                run();
};

#endif // TCPCONNECTIONSERVER_H
