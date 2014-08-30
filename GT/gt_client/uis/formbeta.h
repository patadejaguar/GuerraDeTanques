#ifndef FORMBETA_H
#define FORMBETA_H

#include "form.h"

namespace Ui {
    class FormBeta;
}

class FormBeta : public Form
{
        Q_OBJECT

    public:
        explicit FormBeta(QWidget *parent = 0);
        ~FormBeta();

    private:
        Ui::FormBeta *ui;
};

#endif // FORMBETA_H
