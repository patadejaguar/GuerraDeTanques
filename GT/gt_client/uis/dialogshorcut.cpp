#include "dialogshorcut.h"
#include "ui_dialogshorcut.h"

DialogShorcut::DialogShorcut(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::DialogShorcut)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
}

DialogShorcut::~DialogShorcut()
{
    delete ui;
}

void DialogShorcut::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        reject();
    }
    else
    {
        emit notifyKey(event->key());
        accept();
    }
}

void DialogShorcut::mousePressEvent(QMouseEvent *event)
{
    emit notifyKey(event->button());
    accept();
}
