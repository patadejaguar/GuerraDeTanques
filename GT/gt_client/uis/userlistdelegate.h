#ifndef USERLISTDELEGATE_H
#define USERLISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>

#include "gameglobals.h"

class UserListDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit    UserListDelegate(QObject *parent = 0);

        void        paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

        QSize       sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

};

#endif // USERLISTDELEGATE_H
