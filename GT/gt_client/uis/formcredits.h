#ifndef FORMCREDITS_H
#define FORMCREDITS_H

#include "form.h"

namespace Ui {
    class FormCredits;
}

class FormCredits : public Form
{
        Q_OBJECT

    public:

        explicit FormCredits(QWidget *parent = 0);
        ~FormCredits();

    private slots:

        void                on__pb_back_clicked();

    private:

        Ui::FormCredits *   _ui;
};

#endif // FORMCREDITS_H
