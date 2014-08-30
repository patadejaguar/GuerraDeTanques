#ifndef FORMEDITUSERACCOUNT_H
#define FORMEDITUSERACCOUNT_H

#include "form.h"

namespace Ui {
    class FormEditUserAccount;
}

class FormEditUserAccount : public Form
{
        Q_OBJECT

    public:

        explicit                    FormEditUserAccount(QWidget *parent = 0);

        ~FormEditUserAccount();

    public slots:

        void                        show();

        void                        updateAvatar(QPixmap avatar);

    private slots:

        void                        on__pb_cancel_clicked();

        void                        on__pb_examine_clicked();

        void                        on__pb_accept_clicked();

        void                        on__le_user_textEdited(const QString &arg1);

        void                        on__le_current_password_textEdited(const QString &arg1);

        void                        on__le_new_password_textEdited(const QString &arg1);

        void                        on__le_confirm_password_textEdited(const QString &arg1);

        void                        checkAccept();

    private:

        Ui::FormEditUserAccount *   _ui;

        QPixmap                     _avatar;
};

#endif // FORMEDITUSERACCOUNT_H
