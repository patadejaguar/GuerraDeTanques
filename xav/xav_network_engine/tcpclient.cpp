#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent): TcpConnection(parent)
{
}

void TcpClient::connectToHost(QHostAddress host_address, qint16 port, bool use_ssl)
{
    _use_ssl = use_ssl;
    _host_address = host_address;
    _port = port;
    start();
}

void TcpClient::run()
{
    //create and connect socket
    QAbstractSocket *socket;
//    if(_use_ssl)
//    {
//        socket = new QSslSocket();
//        ((QSslSocket*)socket)->ignoreSslErrors();
//        ((QSslSocket*)socket)->connectToHostEncrypted(_host_address.toString(), _port);
//        if(!((QSslSocket*)socket)->waitForEncrypted(3000))
//        {
//            emit notifyError(tr("No se pudo establecer la conexión segura con el servidor."));
//            stop();
//            socket->close();
//            delete socket;
//            return;
//        }
//    }
//    else
//    {
        socket = new QTcpSocket();
        ((QTcpSocket*)socket)->connectToHost(_host_address, _port);
        if(!((QTcpSocket*)socket)->waitForConnected())
        {
            emit notifyError(tr("No se pudo establecer la conexión con el servidor."));
            stop();
            socket->close();
            delete socket;
            return;
        }
//    }

    emit notifyConnected();

    //read and write socket
    while(_running)
    {        
        while(socket->waitForReadyRead(1))
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
            emit notifyError(tr("Se ha perdido la conexión con el servidor."));
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
