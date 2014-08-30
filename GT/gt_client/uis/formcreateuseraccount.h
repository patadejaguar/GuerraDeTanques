#ifndef FORMCREATEUSERACCOUNT_H
#define FORMCREATEUSERACCOUNT_H

#include "form.h"
#include "connectiontcp.h"

#include <QImageWriter>
#include <QBuffer>

namespace Ui {
    class FormCreateUserAccount;
}

class FormCreateUserAccount : public Form
{
        Q_OBJECT

    public:

        explicit                    FormCreateUserAccount(QWidget *parent = 0);

        ~FormCreateUserAccount();

    public slots:

        void                        show();

        void                        updateAvatar(QPixmap avatar);

    private slots:

        void                        on__pb_cancel_clicked();

        void                        on__le_user_textEdited(const QString &arg1);

        void                        on__le_password_textEdited(const QString &arg1);

        void                        on__le_confirm_password_textEdited(const QString &arg1);

        void                        on__pb_accept_clicked();

        void                        on__pb_examine_clicked();

        void                        checkAccept();

    private:

        Ui::FormCreateUserAccount * _ui;

        QPixmap                     _avatar;
};

#endif // FORMCREATEUSERACCOUNT_H
