#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "tcpconnection.h"

#include <QTcpSocket>
//#include <QSslSocket>
#include <QDataStream>

#include "xav_network_engine_global.h"

class XAV_NETWORK_ENGINESHARED_EXPORT TcpClient : public TcpConnection
{
        Q_OBJECT

    public:

        explicit            TcpClient(QObject *parent=0);

        void                connectToHost(QHostAddress host_address, qint16 port, bool use_ssl = false);

    signals:

        void                notifyError(QString notifyError, bool critical = true);

    protected:

        void                run();

        virtual void        parseData(QDataStream &stream_input) = 0;

        //------------------_att...

        bool                _use_ssl;

        QHostAddress        _host_address;

        qint16              _port;
};

#endif // TCPCLIENT_H
