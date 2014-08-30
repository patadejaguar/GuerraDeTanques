#ifndef TCPServer_H
#define TCPServer_H

#include <QTcpServer>
#include <QList>
#include <QDataStream>
#include <QByteArray>
#include <QSslSocket>

#include "xav_network_engine_global.h"
#include "tcpconnectionserver.h"

class XAV_NETWORK_ENGINESHARED_EXPORT TcpServer :public QTcpServer
{
        Q_OBJECT

    public:

        explicit TcpServer(bool use_ssl = false, QObject *parent = 0);

        ~TcpServer();

        bool                    useSSL();

    protected:

        void                    incomingConnection(int handle) = 0;

        void                    addConnection(TcpConnectionServer* tcp_connection);

    public slots:

        void                    close();

    protected slots:

        void                    newConnection();

        virtual void            closeConnection();

        virtual void            closeConnection(TcpConnectionServer* tcp_connection_server);

    signals:

        void                    notifyNewConnection(TcpConnectionServer*);

        void                    notifyError(QString error);

    protected:

        //----------------------_att...

        QList< TcpConnection* > _tcp_connections;

        bool                    _use_ssl;
};

#endif // TCPServer_H
