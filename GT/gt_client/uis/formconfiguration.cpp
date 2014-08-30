#include "formconfiguration.h"
#include "ui_formconfiguration.h"
#include "forms.h"

#include <QSettings>

FormConfiguration::FormConfiguration(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormConfiguration)
{
    _ui->setupUi(this);
}

FormConfiguration::~FormConfiguration()
{
    delete _ui;
}

void FormConfiguration::show()
{
    _ui->_cb_logs->setChecked(Configuration::activateLogs());
    Form::show();
}

void FormConfiguration::on__pb_controls_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_configuration->hide();
    forms->_form_controls->show();
}

void FormConfiguration::on__pb_sound_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_configuration->hide();
    forms->_form_sound->show();
}

void FormConfiguration::on__pb_network_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_configuration->hide();
    forms->_form_network->show();
}

void FormConfiguration::on__pb_restore_clicked()
{
    Dialog* dialog = new Dialog("¿Estás seguro que quieres restablecer la configuración?");

    if(dialog->exec() == QDialog::Accepted)
    {
        Configuration::restore();
        _ui->_cb_logs->setChecked(Configuration::activateLogs());
        Configuration::save();
    }
}

void FormConfiguration::on__pb_back_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_configuration->hide();
    forms->_form_principal->show();
}

void FormConfiguration::on__cb_logs_clicked(bool checked)
{
    Configuration::setActivateLogs(checked);
    Configuration::save();
}
