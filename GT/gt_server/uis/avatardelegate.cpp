#include "avatardelegate.h"

AvatarDelegate::AvatarDelegate(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void AvatarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(option.state & QStyle::State_Selected)
        painter->fillRect( option.rect, option.palette.highlight() );

    QPixmap value;
    value.loadFromData(index.model()->data(index).toByteArray());

    int h = option.rect.height();
    int w = h;
    painter->drawPixmap(option.rect.x()+option.rect.width()/2-(h/2),option.rect.y(),w,h,value);
}

QSize AvatarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(50,50);
}

QWidget *AvatarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QPushButton* editor= new QPushButton(parent);
    editor->setText("Cambiar avatar");
    connect(dynamic_cast<QPushButton*>(editor), &QPushButton::clicked, this, &AvatarDelegate::loadAvatar);

    return editor;
}

void AvatarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);

    if(!_file_image.isEmpty())
    {
        QByteArray data;
        QBuffer buffer(&data);

        QPixmap pixmap;
        if(pixmap.load(_file_image))
        {
            QPixmap scaled_pixmap = pixmap.scaled(50,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

            if(!scaled_pixmap.isNull())
            {
                scaled_pixmap.save(&buffer,"JPG");
                model->setData(index, data);
            }
        }
    }
}

void AvatarDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

void AvatarDelegate::loadAvatar()
{
    _file_image = QFileDialog::getOpenFileName(0,"Seleccionar imagen","","Imagen (*.jpg)");
}
