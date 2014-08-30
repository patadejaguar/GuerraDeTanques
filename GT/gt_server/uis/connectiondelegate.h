#ifndef CONNECTIONDELEGATE_H
#define CONNECTIONDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>

class ConnectionDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit ConnectionDelegate(QObject *parent = 0);

        void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

        QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

};

#endif // CONNECTIONDELEGATE_H
