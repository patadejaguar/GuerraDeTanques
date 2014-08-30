#ifndef FORMPRINCIPAL_H
#define FORMPRINCIPAL_H

#include "form.h"
#include "formbeta.h"

namespace Ui {
    class FormPrincipal;
}

class FormPrincipal : public Form
{
        Q_OBJECT

    public:

        explicit FormPrincipal(QWidget *parent = 0);

        ~FormPrincipal();

    public slots:

        void                show();

        void                hide();

    private slots:

        void                on__pb_play_clicked();

        void                on__pb_configuration_clicked();

        void                on__pb_help_clicked();

        void                on__pb_credits_clicked();

        void                on__pb_exit_clicked();

        void                on__pb_replays_clicked();

    private:

        Ui::FormPrincipal * _ui;

        FormBeta*           _form_beta;
};

#endif // FORMPRINCIPAL_H
