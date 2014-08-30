#include "formcredits.h"
#include "ui_formcredits.h"
#include "forms.h"

FormCredits::FormCredits(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormCredits)
{
    _ui->setupUi(this);
}

FormCredits::~FormCredits()
{
    delete _ui;
}

void FormCredits::on__pb_back_clicked()
{
    hide();
    Forms::instance()->_form_principal->show();
}
