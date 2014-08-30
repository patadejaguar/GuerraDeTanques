#include "dialogconfiguration.h"
#include "ui_dialogconfiguration.h"
#include "serverconfiguration.h"

DialogConfiguration::DialogConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfiguration)
{
    ui->setupUi(this);
}

DialogConfiguration::~DialogConfiguration()
{
    delete ui;
}

int DialogConfiguration::exec()
{
    ServerConfiguration *configuration = ServerConfiguration::instance();
    ui->checkBox->setChecked(configuration->useSsl());
    ui->spinBox->setValue(configuration->getPort());
    return QDialog::exec();
}


void DialogConfiguration::on_pushButton_2_clicked()
{
    reject();
}

void DialogConfiguration::on_pushButton_clicked()
{
    ServerConfiguration *configuration = ServerConfiguration::instance();
    configuration->setUseSsl(ui->checkBox->isChecked());
    configuration->setPort(ui->spinBox->value());
    accept();
}
