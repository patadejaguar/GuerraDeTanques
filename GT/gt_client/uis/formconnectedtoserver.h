#ifndef FORMCONNECTEDTOSERVER_H
#define FORMCONNECTEDTOSERVER_H

#include "form.h"

namespace Ui {
    class FormConnectedToServer;
}

class FormConnectedToServer : public Form
{
        Q_OBJECT

    public:

        explicit                    FormConnectedToServer(QWidget *parent = 0);

        ~FormConnectedToServer();

    public slots:

        void                        show();

        void                        show(QString user);

    private slots:

        void                        on__pb_accept_clicked();

        void                        on__pb_cancel_clicked();

        void                        on__pb_create_user_clicked();

        void                        on__pb_edit_user_clicked();

    private:

        Ui::FormConnectedToServer*  _ui;
};

#endif // FORMCONNECTEDTOSERVER_H
