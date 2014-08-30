#include "form.h"

Form::Form(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

void Form::closeEvent(QCloseEvent *)
{
    qApp->quit();
}

void Form::disable()
{
    setEnabled(false);
}

void Form::enable()
{
    setEnabled(true);
}

void Form::show()
{
    enable();
    QWidget::show();
}

void Form::hide()
{
    QWidget::hide();
}
