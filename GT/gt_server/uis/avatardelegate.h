#ifndef AVATARDELEGATE_H
#define AVATARDELEGATE_H

#include <QAbstractItemDelegate>
#include <QPainter>
#include <QFileDialog>
#include <QPushButton>
#include <QBuffer>
#include <QPixmap>

class AvatarDelegate : public QAbstractItemDelegate
{
        Q_OBJECT

    public:

        explicit AvatarDelegate(QObject *parent = 0);

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

        QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

        void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    private slots:

        void loadAvatar();

    private:

        QString _file_image;

};

#endif // AVATARDELEGATE_H
