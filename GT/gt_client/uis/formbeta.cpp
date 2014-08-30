#include "formbeta.h"
#include "ui_formbeta.h"

FormBeta::FormBeta(QWidget *parent) :
    Form(parent),
    ui(new Ui::FormBeta)
{
    ui->setupUi(this);
}

FormBeta::~FormBeta()
{
    delete ui;
}
