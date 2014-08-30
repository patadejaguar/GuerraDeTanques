#include "connectionmodel.h"
#include "connection.h"

ConnectionModel::ConnectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void ConnectionModel::addConnection(Connection *connection)
{
    _connections.append(connection);
    connect(connection, &Connection::destroyed, this, &ConnectionModel::removeConnection);
    connect(connection,SIGNAL(notifyRequestLoginUser(QString)), this, SLOT(reset()));
    connect(connection,SIGNAL(notifyLogoutUser(UserProperties)), this, SLOT(reset()));
    endResetModel();
}

void ConnectionModel::removeConnection(QObject *connection)
{
    beginResetModel();
    _connections.removeAll((Connection*)connection);
    endResetModel();
}

void ConnectionModel::reset()
{
    beginResetModel();
    endResetModel();
}

int ConnectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _connections.size();
}

QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        Connection *connection = _connections.at(index.row());
        return QString(connection->address() + "[" +connection->userProperties()._nick + "]");
    }
    return QVariant();
}
