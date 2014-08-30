#ifndef FORMCONFIGURATION_H
#define FORMCONFIGURATION_H

#include "form.h"
#include "configuration.h"
#include "dialog.h"

namespace Ui {
    class FormConfiguration;
}

class FormConfiguration : public Form
{
        Q_OBJECT

    public:

        explicit            FormConfiguration(QWidget *parent = 0);

        ~FormConfiguration();

    private slots:

        void                show();

        void                on__pb_controls_clicked();

        void                on__pb_sound_clicked();

        void                on__pb_network_clicked();

        void                on__pb_restore_clicked();

        void                on__pb_back_clicked();

        void                on__cb_logs_clicked(bool checked);

    private:

        Ui::FormConfiguration*  _ui;
};

#endif // FORMCONFIGURATION_H
