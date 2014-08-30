#ifndef TERRAINLISTDELEGATE_H
#define TERRAINLISTDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QStringListModel>

class TerrainListDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit TerrainListDelegate(QObject *parent = 0);

        void        paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

        QSize       sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;

};

#endif // TERRAINLISTDELEGATE_H
