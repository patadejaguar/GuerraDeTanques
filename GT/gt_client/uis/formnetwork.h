#ifndef FORMNETWORK_H
#define FORMNETWORK_H

#include "form.h"

namespace Ui {
    class FormNetwork;
}

class FormNetwork : public Form
{
        Q_OBJECT

    public:

        explicit FormNetwork(QWidget *parent = 0);

        ~FormNetwork();

    public slots:

        void                show();

    private slots:

        void                on__pb_accept_clicked();

        void                on__pb_cancel_clicked();

    private:
        Ui::FormNetwork *   _ui;
};

#endif // FORMNETWORK_H
