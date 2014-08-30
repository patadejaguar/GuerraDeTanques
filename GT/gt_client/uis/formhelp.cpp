#include "formhelp.h"
#include "ui_formhelp.h"
#include "forms.h"

FormHelp::FormHelp(QWidget *parent) :
    Form(parent),
    ui(new Ui::FormHelp)
{
    ui->setupUi(this);
}

FormHelp::~FormHelp()
{
    delete ui;
}

void FormHelp::on__pb_back_clicked()
{
    hide();
    Forms::instance()->_form_principal->show();
}
