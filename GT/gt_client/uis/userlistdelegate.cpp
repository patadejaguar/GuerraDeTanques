#include "userlistdelegate.h"
#include "userlistmodel.h"
#include "connectiontcp.h"

UserListDelegate::UserListDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void UserListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, QBrush(QColor(180, 190, 140, 20)));

    UserProperties user_login = ((UserListModel*)index.model())->getUserLogin(index.row());

    painter->setFont(QFont("REVUE BT",8));
    painter->drawText(option.rect.x()+65, option.rect.y()+18, user_login._nick);

    int w_h = option.rect.height()-4;
    if(!user_login._avatar.isNull())
        painter->drawPixmap(option.rect.x()+28, option.rect.y()+2, w_h, w_h, user_login._avatar);
    else
        painter->drawRect(option.rect.x()+28, option.rect.y()+2, w_h, w_h);

    painter->drawPixmap(option.rect.x()+2, option.rect.y()+3, w_h-6, w_h-3, QPixmap(QString(":/gt/ranks/rank_%1.png").arg(user_login.rank())));
}

QSize UserListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    UserProperties user_login = ((UserListModel*)index.model())->getUserLogin(index.row());
    return QSize(option.fontMetrics.width( user_login._nick )+40, 30);
}
