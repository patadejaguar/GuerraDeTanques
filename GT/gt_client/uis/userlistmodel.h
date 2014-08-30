#ifndef USERLISTMODEL_H
#define USERLISTMODEL_H

#include "connectiontcp.h"
#include <QAbstractListModel>

class UserListModel : public QAbstractListModel
{
        Q_OBJECT

    public:

        explicit                UserListModel(QObject *parent = 0);

        QVariant                data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

        int                     rowCount( const QModelIndex &parent = QModelIndex() ) const;

        inline UserProperties   getUserLogin(int index){return _users_login.at(index);}

        inline void             clear(){_users_login.clear();}

    public slots:

        void                    setUsersLogins(QList<UserProperties> users_login);

        void                    addUsersLogin(QList<UserProperties> users_login);

        void                    addUserLogin(UserProperties user_login);

        void                    removeUser(UserProperties user);        

    signals:

    public slots:

    private:

        QList<UserProperties>   _users_login;
};

#endif // USERLISTMODEL_H
