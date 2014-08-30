#include "tcpconnectionserver.h"

TcpConnectionServer::TcpConnectionServer(bool use_ssl, int socket_descriptor, QObject *parent):
    TcpConnection(use_ssl, socket_descriptor, parent)
{
}

void TcpConnectionServer::run()
{
    //create a socket
    QAbstractSocket *socket;
//    if(_use_ssl)
//    {
//        socket = new QSslSocket();
//        ((QSslSocket*)socket)->ignoreSslErrors();
//        if(!((QSslSocket*)socket)->setSocketDescriptor(_socket_descriptor))
//        {
//            stop();
//            socket->close();
//            delete socket;
//            emit notifyDisconnected();
//            return;
//        }
//        ((QSslSocket*)socket)->startServerEncryption();
//        if(!((QSslSocket*)socket)->waitForEncrypted())
//        {
//            stop();
//            socket->close();
//            delete socket;
//            emit notifyDisconnected();
//            return;
//        }
//    }
//    else
//    {
        socket = new QTcpSocket();
        if(!((QTcpSocket*)socket)->setSocketDescriptor(_socket_descriptor))
        {
            socket->close();
            delete socket;
            emit notifyDisconnected();
            return;
        }
//    }

    _address = socket->peerAddress().toString();
    _port = socket->peerPort();
    emit notifyConnected();

    //read and write socket
    while(_running)
    {
        if(socket->waitForReadyRead(1))
        {
            QByteArray data;
            data.resize(TCP_SIZE_OF_DATA);
            socket->read(data.data(), TCP_SIZE_OF_DATA);
            int size = data.toInt();
            data.clear();
            while(data.size() < size)
            {
                QByteArray data_aux;
                data_aux.resize(data_aux.size()+socket->bytesAvailable());
                socket->read(data_aux.data(), data_aux.size());
                data.append(data_aux);
                if(data.size() < size)
                    socket->waitForReadyRead(1);
            }
            QDataStream stream(&data,QIODevice::ReadOnly);
            parseData(stream);
        }

        if(socket->state() == QAbstractSocket::UnconnectedState)
        {
            stop();
        }

        _mutex.lock();
        if(!_buffer_data_to_write.isEmpty())
        {
            QByteArray data_to_write = _buffer_data_to_write.takeAt(0);

            socket->write(data_to_write,data_to_write.size());

            socket->waitForBytesWritten();
        }
        _mutex.unlock();
    }

    emit notifyDisconnected();
    socket->close();
    delete socket;
    _buffer_data_to_write.clear();
}
