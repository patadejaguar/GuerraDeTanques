#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
        Q_OBJECT

    public:
        explicit Dialog(QString text, QWidget *parent = 0);
        ~Dialog();

    private slots:
        void on__pb_accept_clicked();

        void on__pb_cancel_clicked();

    private:
        Ui::Dialog *ui;
};

#endif // DIALOG_H
