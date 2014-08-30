#ifndef FORMDIALOGMESSAGE_H
#define FORMDIALOGMESSAGE_H

#include "form.h"

namespace Ui {
    class FormDialogMessage;
}

class FormDialogMessage : public Form
{
        Q_OBJECT

    public:

        explicit                FormDialogMessage(QWidget *parent = 0);

        ~FormDialogMessage();

    public slots:

        void                    show(/*void ((* call_back)()),*/ QString title, QString text);

        void                    showInformation(QString title, QString information);

        void                    showError(QString title, QString error, bool critical);

    private slots:

        void                    closeCritical();

        void                    closeNoCritical();

        void                    on__pb_cancel_clicked();

    private:

        Ui::FormDialogMessage * _ui;

        bool                    _critical;
};

#endif // FORMDIALOGMESSAGE_H
