#ifndef PASSWORDDELEGATE_H
#define PASSWORDDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QLineEdit>

class PasswordDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit PasswordDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    signals:

    public slots:

};

#endif // PASSWORDDELEGATE_H
