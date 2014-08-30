#include "formsound.h"
#include "ui_formsound.h"
#include "forms.h"

FormSound::FormSound(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormSound)
{
    _ui->setupUi(this);
}

FormSound::~FormSound()
{
    delete _ui;
}

void FormSound::show()
{
    _ui->_hs_volume_music->setValue(Configuration::volumeMusic());
    _ui->_hs_volume_sound->setValue(Configuration::volumeSound());
    Form::show();
}

void FormSound::on__pb_accept_clicked()
{
    Configuration::setVolumeMusic(_ui->_hs_volume_music->value());
    Configuration::setVolumeSound(_ui->_hs_volume_sound->value());
    Configuration::save();

    Forms* forms = Forms::instance();
    forms->_form_sound->hide();
    forms->_form_configuration->show();
}

void FormSound::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();
    forms->_form_sound->hide();
    forms->_form_configuration->show();
}
