#ifndef FORMSOUND_H
#define FORMSOUND_H

#include "form.h"

namespace Ui {
    class FormSound;
}

class FormSound : public Form
{
        Q_OBJECT

    public:

        explicit FormSound(QWidget *parent = 0);

        ~FormSound();

    public slots:

        void            show();

    private slots:

        void            on__pb_accept_clicked();

        void            on__pb_cancel_clicked();

    private:

        Ui::FormSound * _ui;
};

#endif // FORMSOUND_H
