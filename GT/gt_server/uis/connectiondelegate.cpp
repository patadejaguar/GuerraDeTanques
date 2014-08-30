#include "connectiondelegate.h"
#include "connectionmodel.h"
#include "connection.h"

ConnectionDelegate::ConnectionDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void ConnectionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect( option.rect, option.palette.highlight() );

    Connection *connection = ((ConnectionModel*)index.model())->getConnection(index.row());

    painter->drawText(option.rect.x()+35,option.rect.y()+12,QString("[%1]").arg(connection->address()));
    if(connection->isLogin())
        painter->drawText(option.rect.x()+35,option.rect.y()+25,QString("%1 : %2").arg(connection->userProperties()._nick).arg(connection->location()));

    QPixmap avatar = connection->userProperties()._avatar;
    int w_h = option.rect.height()-4;
    if(!avatar.isNull())
    {
        painter->drawPixmap(option.rect.x()+2,option.rect.y()+2,w_h,w_h,avatar);
    }
    else
        painter->drawRect(option.rect.x()+2,option.rect.y()+2,w_h,w_h);
}

QSize ConnectionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Connection *connection = ((ConnectionModel*)index.model())->getConnection(index.row());
    return QSize(option.fontMetrics.width( connection->address() )+40, 30);
}
