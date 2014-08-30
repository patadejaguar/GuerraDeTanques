#include "formcontrols.h"
#include "ui_formcontrols.h"
#include "forms.h"

FormControls::FormControls(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormControls)
{
    _ui->setupUi(this);

    _current_button = NULL;

    _buttons.append(_ui->_pb_forward);
    _buttons.append(_ui->_pb_backward);
    _buttons.append(_ui->_pb_turn_left);
    _buttons.append(_ui->_pb_turn_right);
    _buttons.append(_ui->_pb_skill_1);
    _buttons.append(_ui->_pb_skill_2);
    _buttons.append(_ui->_pb_skill_3);
    _buttons.append(_ui->_pb_update_1);
    _buttons.append(_ui->_pb_update_2);
    _buttons.append(_ui->_pb_update_3);

    foreach (QPushButton* button, _buttons)
        connect(button, &QPushButton::clicked, this, &FormControls::selectControl);

    _dialog_shorcut = new DialogShorcut(this);
    connect(_dialog_shorcut, &DialogShorcut::notifyKey, this, &FormControls::shortcut);
}

FormControls::~FormControls()
{
    delete _ui;
}

void FormControls::show()
{
    updateControls();
    Form::show();
}

void FormControls::shortcut(int key)
{
    if(_current_button != NULL)
    {
        _controls.replace(_buttons.indexOf(_current_button), key);
        _current_button = NULL;
        checkControls();
    }
}

void FormControls::selectControl()
{
    QPushButton* button = dynamic_cast<QPushButton*>(sender());
    if(button)
    {
        _current_button = button;
        setFocus();
        _dialog_shorcut->exec();
    }
}

void FormControls::updateControls()
{
    _controls = Configuration::controls();
    checkControls();
}

void FormControls::checkControls()
{
    bool accept = true;
    for (int i = 0; i < _controls.size(); ++i)
    {
        _buttons.at(i)->setText(keyNameFromValue(_controls.at(i)));
        if(_controls.count(_controls.at(i)) > 1)
        {
            _buttons.at(i)->setStyleSheet("color: red;");
            accept = false;
        }
        else
            _buttons.at(i)->setStyleSheet("color: white;");
    }
    _ui->_pb_accept->setEnabled(accept);
}

void FormControls::on__pb_accept_clicked()
{
    Configuration::setControls(_controls);
    Configuration::save();

    Forms* forms = Forms::instance();
    forms->_form_controls->hide();
    forms->_form_configuration->show();
}

void FormControls::on__pb_cancel_clicked()
{
    Forms* forms = Forms::instance();
    forms->_form_controls->hide();
    forms->_form_configuration->show();
}

QString FormControls::keyNameFromValue(const int key)
{
    if (key == Qt::NoButton)      return "";
    if (key == Qt::LeftButton)    return "Click Izq";
    if (key == Qt::RightButton)   return "Click Der";
    if (key == Qt::MiddleButton)  return "Click Med";
    return QKeySequence(key).toString();
}
