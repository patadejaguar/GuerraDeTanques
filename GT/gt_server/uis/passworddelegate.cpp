#include "passworddelegate.h"
#include <QCryptographicHash>

PasswordDelegate::PasswordDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void PasswordDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());

    QString value;
    value = index.model()->data(index).toString();

    painter->drawText(option.rect, value);
}

QSize PasswordDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return option.rect.size();
}

QWidget *PasswordDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *editor = new QLineEdit(parent);
    editor->setAutoFillBackground(true);
//    editor->installEventFilter(const_cast<PasswordDelegate*>(this));
    return editor;
}

void PasswordDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, QString(QCryptographicHash::hash(static_cast<QLineEdit*>(editor)->text().toUtf8(), QCryptographicHash::Sha3_512).toBase64()));
}

void PasswordDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
