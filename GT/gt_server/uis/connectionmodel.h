#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <connection.h>
#include <QAbstractListModel>

class ConnectionModel : public QAbstractListModel
{
        Q_OBJECT

    public:

        explicit ConnectionModel(QObject *parent = 0);

        QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

        int rowCount( const QModelIndex &parent = QModelIndex() ) const;

        inline Connection* getConnection(int index){return _connections.at(index);}

    public slots:

        void addConnection(Connection *connection);

        void removeConnection(QObject *connection);

    protected slots:

        void                reset();

    private:

        QList<Connection*> _connections;
};

#endif // CONNECTIONMODEL_H
