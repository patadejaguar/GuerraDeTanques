#include "serverstatus.h"
#include "ui_serverstatus.h"

ServerStatus::ServerStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerStatus)
{
    ui->setupUi(this);
}

ServerStatus::~ServerStatus()
{
    delete ui;
}
