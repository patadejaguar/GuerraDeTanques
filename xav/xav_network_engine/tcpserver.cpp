#include "tcpserver.h"

TcpServer::TcpServer(bool use_ssl, QObject *parent)
    :QTcpServer(parent)
{
    _use_ssl = use_ssl;
    if(_use_ssl /*&& !QSslSocket::supportsSsl()*/)
        emit notifyError(QString(tr("El sistema no soporta SSL.")));
}

TcpServer::~TcpServer()
{
    foreach (TcpConnection *tcp_connection, _tcp_connections)
        while(tcp_connection->isRunning())
        {
            tcp_connection->stop();
            tcp_connection->wait();
        }
    qDeleteAll(_tcp_connections);
}

bool TcpServer::useSSL()
{
    return _use_ssl;
}

void TcpServer::addConnection(TcpConnectionServer *tcp_connection)
{
    connect(tcp_connection, &TcpConnectionServer::notifyConnected, this, &TcpServer::newConnection);
    connect(tcp_connection, SIGNAL(notifyDisconnected()), this, SLOT(closeConnection()));

    _tcp_connections.append(tcp_connection);
    tcp_connection->start();
}

void TcpServer::closeConnection()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;
    TcpConnectionServer *tcp_connection_server = (TcpConnectionServer*)sender_object;
    while(tcp_connection_server->isRunning())
    {
        tcp_connection_server->stop();
        tcp_connection_server->wait();
    }
    _tcp_connections.removeOne(tcp_connection_server);
    delete tcp_connection_server;
}

void TcpServer::closeConnection(TcpConnectionServer *tcp_connection_server)
{
    while(tcp_connection_server->isRunning())
    {
        tcp_connection_server->stop();
        tcp_connection_server->wait();
    }
    _tcp_connections.removeAll(tcp_connection_server);
    delete tcp_connection_server;
}

void TcpServer::close()
{
    foreach (TcpConnection *tcp_connection, _tcp_connections)
        while(tcp_connection->isRunning())
        {
            tcp_connection->stop();
            tcp_connection->wait();
        }
    qDeleteAll(_tcp_connections);
    _tcp_connections.clear();
    QTcpServer::close();
}

void TcpServer::newConnection()
{
    QObject *sender_object = sender();
    if(sender_object == 0)
        return;
    emit notifyNewConnection((TcpConnectionServer*)sender_object);
}
