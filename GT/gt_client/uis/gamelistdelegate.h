#ifndef GAMELISTDELEGATE_H
#define GAMELISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QStringListModel>

class GameListDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit GameListDelegate(QObject *parent = 0);

        void        paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

        QSize       sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

};

#endif // GAMELISTDELEGATE_H
