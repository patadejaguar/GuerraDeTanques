#include "tcpconnection.h"

TcpConnection::TcpConnection(bool use_ssl, int socket_descriptor, QObject *parent):
    QThread(parent)
{
    _use_ssl = use_ssl;
    _socket_descriptor = socket_descriptor;
    _running = false;
}
TcpConnection::TcpConnection(bool use_ssl, QObject *parent):
    QThread(parent)
{
    _use_ssl = use_ssl;
    _socket_descriptor = -1;
    _running = false;
}
TcpConnection::TcpConnection(int socket_descriptor, QObject *parent):
    QThread(parent)
{
    _use_ssl = false;
    _socket_descriptor = socket_descriptor;
    _running = false;
}

QString TcpConnection::address()
{
    return _address;
}

qint16 TcpConnection::getPort()
{
    return _port;
}

void TcpConnection::start()
{
    _running = true;
    QThread::start();
}

void TcpConnection::stop()
{
    _running = false;
}

void TcpConnection::sendData(QByteArray data)
{
    QMutexLocker locker(&_mutex);
    _buffer_data_to_write.append(data.prepend(QByteArray::number(data.size()).rightJustified(TCP_SIZE_OF_DATA, '0')));
}

