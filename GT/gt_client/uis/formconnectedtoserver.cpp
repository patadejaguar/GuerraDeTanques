#include "formconnectedtoserver.h"
#include "ui_formconnectedtoserver.h"
#include "connectiontcp.h"
#include "configuration.h"
#include "forms.h"

#include <QMessageBox>

FormConnectedToServer::FormConnectedToServer(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormConnectedToServer)
{
    _ui->setupUi(this);
}

FormConnectedToServer::~FormConnectedToServer()
{
    delete _ui;
}

void FormConnectedToServer::show()
{
    Form::show();
    _ui->_le_password->clear();
    if(_ui->_le_user->text().isEmpty())
    {
        QSettings settings("game_config", QSettings::IniFormat);
        QString nick = settings.value("last_nick_login", "").toString();
        if(nick == "")
            _ui->_le_user->setFocus();
        else
        {
            _ui->_le_user->setText(nick);
            _ui->_le_password->setFocus();
        }

    }
    else
        _ui->_le_password->setFocus();
}

void FormConnectedToServer::show(QString user)
{
    Form::show();
    _ui->_le_user->setText(user);
    _ui->_le_password->clear();
    _ui->_le_password->setFocus();
}

void FormConnectedToServer::on__pb_accept_clicked()
{
    ConnectionTcp *connection_tcp = ConnectionTcp::instance();
    connection_tcp->sendLoginUser(_ui->_le_user->text(), _ui->_le_password->text());
}

void FormConnectedToServer::on__pb_cancel_clicked()
{
    ConnectionTcp *connection_tcp = ConnectionTcp::instance();
    while(connection_tcp->isRunning())
    {
        connection_tcp->stop();
        connection_tcp->wait();
    }

    Forms* forms = Forms::instance();

    forms->_form_connected_to_server->hide();
    forms->_form_principal->show();
}

void FormConnectedToServer::on__pb_create_user_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_connected_to_server->hide();
    forms->_form_create_user_account->show();
}

void FormConnectedToServer::on__pb_edit_user_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_connected_to_server->hide();
    forms->_form_edit_user_account->show();
}

