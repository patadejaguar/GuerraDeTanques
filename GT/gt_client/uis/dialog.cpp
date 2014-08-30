#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QString text, QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->_l_text->setText(text);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on__pb_accept_clicked()
{
    accept();
}

void Dialog::on__pb_cancel_clicked()
{
    reject();
}
