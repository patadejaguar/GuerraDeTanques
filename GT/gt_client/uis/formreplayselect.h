#ifndef FORMREPLAYSELECT_H
#define FORMREPLAYSELECT_H

#include <QStringListModel>

#include "form.h"

namespace Ui {
    class FormReplaySelect;
}

class FormReplaySelect : public Form
{
        Q_OBJECT

    public:

        explicit FormReplaySelect(QWidget *parent = 0);
        ~FormReplaySelect();

    public slots:

        void                    show();

    private slots:

        void                    on__pb_cancel_clicked();

        void on__pb_accept_clicked();

    private:

        Ui::FormReplaySelect *  _ui;

        QStringListModel*       _model;
};

#endif // FORMREPLAYSELECT_H
