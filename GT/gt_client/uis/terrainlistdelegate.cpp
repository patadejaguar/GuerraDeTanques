#include "terrainlistdelegate.h"

TerrainListDelegate::TerrainListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void TerrainListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, QBrush(QColor(180, 190, 140, 20)));

    int w_h = option.rect.height()-4;
    painter->drawPixmap(option.rect.x()+2, option.rect.y()+2, w_h, w_h, QPixmap(":/icons/terrain_icon.png"));

    QString terrain = ((QStringListModel*)index.model())->stringList().at(index.row());

    //painter->setFont(QFont("REVUE BT",8));
    painter->drawText(option.rect.x()+35, option.rect.y()+18, terrain);
}

QSize TerrainListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString terrain = ((QStringListModel*)index.model())->stringList().at(index.row());
    return QSize(option.fontMetrics.width( terrain ) + 40, 30);
}
