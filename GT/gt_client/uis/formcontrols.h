#ifndef FORMCONTROLS_H
#define FORMCONTROLS_H

#include <QPushButton>

#include "form.h"
#include "dialogshorcut.h"

namespace Ui {
    class FormControls;
}

class FormControls : public Form
{
        Q_OBJECT

    public:

        explicit FormControls(QWidget *parent = 0);

        ~FormControls();

    public slots:

        void                show();

    private slots:

        void                shortcut(int key);

        void                selectControl();

        void                updateControls();

        void                checkControls();

        void                on__pb_accept_clicked();

        void                on__pb_cancel_clicked();

    private:

        QString keyNameFromValue(const int key);

        Ui::FormControls *  _ui;

        QList<QPushButton*> _buttons;

        QPushButton*        _current_button;

        QList<int>          _controls;

        DialogShorcut*      _dialog_shorcut;
};

#endif // FORMCONTROLS_H
