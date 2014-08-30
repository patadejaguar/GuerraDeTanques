#include "userlistmodel.h"

UserListModel::UserListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void UserListModel::addUserLogin(UserProperties user_login)
{
    bool no = true;
    for(int i= 0; i< _users_login.size() && no; ++i)
        if(_users_login.at(i)._nick == user_login._nick)
            no = false;
    if(no)
    {
        _users_login.append(user_login);
        endResetModel();
    }
}

void UserListModel::removeUser(UserProperties user)
{
    beginResetModel();
    _users_login.removeAll(user);
    endResetModel();
}

int UserListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _users_login.size();
}

void UserListModel::setUsersLogins(QList<UserProperties> users_login)
{
    beginResetModel();
    _users_login = users_login;
    endResetModel();
}

void UserListModel::addUsersLogin(QList<UserProperties> users_login)
{
    beginResetModel();
    _users_login.append(users_login);
    endResetModel();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        UserProperties user_login = _users_login.at(index.row());
        return QString(user_login._nick);
    }
    return QVariant();
}
