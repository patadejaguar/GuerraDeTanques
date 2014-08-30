#include "formnetwork.h"
#include "ui_formnetwork.h"
#include "forms.h"

FormNetwork::FormNetwork(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormNetwork)
{
    _ui->setupUi(this);
}

FormNetwork::~FormNetwork()
{
    delete _ui;
}

void FormNetwork::show()
{
    _ui->_le_server->setText(Configuration::networkServer());
    _ui->_sb_port->setValue(Configuration::networkServerPort());
    _ui->_cb_use_ssl->setChecked(Configuration::networkUseSsl());
    Form::show();
}

void FormNetwork::on__pb_accept_clicked()
{
    Configuration::setNetworkServer(_ui->_le_server->text());
    Configuration::setNetworkServerPort(_ui->_sb_port->value());
    Configuration::setNetworkUseSsl(_ui->_cb_use_ssl->isChecked());
    Configuration::save();

    Forms* forms = Forms::instance();
    forms->_form_network->hide();
    forms->_form_configuration->show();
}

void FormNetwork::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();
    forms->_form_network->hide();
    forms->_form_configuration->show();
}
