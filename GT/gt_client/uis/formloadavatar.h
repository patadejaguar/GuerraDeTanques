#ifndef FORMLOADAVATAR_H
#define FORMLOADAVATAR_H

#include "form.h"
#include <QDirModel>
#include "filterproxymodel.h"


namespace Ui {
    class FormLoadAvatar;
}

class FormLoadAvatar : public Form
{
        Q_OBJECT

    public:

        explicit            FormLoadAvatar(QWidget *parent = 0);

        ~FormLoadAvatar();

    private slots:

        void                preview(QModelIndex model_index);

        void                on__pb_cancel_clicked();

        void                on__pb_accept_clicked();

    protected:

        bool                eventFilter(QObject *obj, QEvent *event);

    private:

        Ui::FormLoadAvatar* _ui;

        QDirModel*          _dir_model;

        FilterProxyModel*   _proxy_model;

        QPixmap             _pixmap;
};

#endif // FORMLOADAVATAR_H
