#ifndef FORMHELP_H
#define FORMHELP_H

#include "form.h"

namespace Ui {
    class FormHelp;
}

class FormHelp : public Form
{
        Q_OBJECT

    public:
        explicit FormHelp(QWidget *parent = 0);
        ~FormHelp();

    private slots:
        void on__pb_back_clicked();

    private:
        Ui::FormHelp *ui;
};

#endif // FORMHELP_H
