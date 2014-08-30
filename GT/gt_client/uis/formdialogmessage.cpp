#include "formdialogmessage.h"
#include "ui_formdialogmessage.h"
#include "configuration.h"
#include "connectiontcp.h"
#include "forms.h"

#include <QMessageBox>

FormDialogMessage::FormDialogMessage(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormDialogMessage)
{
    _ui->setupUi(this);
}

FormDialogMessage::~FormDialogMessage()
{
    delete _ui;
}

void FormDialogMessage::show(QString title, QString text)
{
    _ui->_l_title->setText(title);
    _ui->_pb_cancel->setVisible(false);
    _ui->_l_icon->setPixmap(QPixmap(":/dialog/icon_information.png"));
    _ui->_l_text->setText(text);
    QWidget::show();
}

//void FormDialogMessage::show()
//{
//    ui->_pb_cancel->setVisible(false);
//    ui->_l_icon->setPixmap(QPixmap(":/dialog/icon_information.png"));
//    Configuration *configuration = Configuration::instance();
//    ConnectionTcp *connection_tcp = ConnectionTcp::instance();
//    while(connection_tcp->isRunning())
//    {
//        connection_tcp->stop();
//        connection_tcp->wait();
//    }
//    ui->_l_title->setText("Conexión");
//    QString security;
//    configuration->useSsl()?security="segura" : security = "insegura";
//    QString server = configuration->getServer();
//    int port = configuration->getPort();
//    ui->_l_text->setText(QString("Conectando al servidor %1 por el puerto %2 usando conexión %3.").arg(server).arg(port).arg(security));

//    connection_tcp->connectToHost(QHostAddress(server),port,configuration->useSsl());

//    QWidget::show();
//}

void FormDialogMessage::showInformation(QString title, QString information)
{
    _ui->_l_title->setText(title);
    _ui->_pb_cancel->setVisible(false);
    _ui->_l_icon->setPixmap(QPixmap(":/dialog/icon_information.png"));
    _ui->_l_text->setText(information);
    QWidget::show();
}

void FormDialogMessage::showError(QString title, QString error, bool critical)
{
    _ui->_l_title->setText(title);
    _critical = critical;
    _ui->_pb_cancel->setVisible(true);
    _ui->_l_text->setText(error);
    _ui->_l_icon->setPixmap(QPixmap(":/dialog/icon_error.png"));
    QWidget::show();
}

void FormDialogMessage::closeCritical()
{
    Forms* forms = Forms::instance();

    forms->_form_principal->show();
    forms->_form_connected_to_server->hide();
    forms->_form_create_user_account->hide();
    forms->_form_edit_user_account->hide();
    forms->_form_load_avatar->hide();
    forms->_form_create_game->hide();
    forms->_form_created_game->hide();
    forms->_form_join_game->hide();
    forms->_form_tank_selector->hide();
    forms->_form_chat->hide();
    forms->_form_game->hide();
    forms->_form_init_game->hide();
    forms->_form_result->hide();
}

void FormDialogMessage::closeNoCritical()
{
    Forms* forms = Forms::instance();

    forms->_form_create_user_account->enable();
    forms->_form_edit_user_account->enable();
    forms->_form_connected_to_server->enable();
    forms->_form_create_game->enable();
    forms->_form_created_game->enable();
    forms->_form_chat->enable();
    forms->_form_join_game->enable();
}

void FormDialogMessage::on__pb_cancel_clicked()
{
    if(_critical)
    {
        ConnectionTcp *connection_tcp = ConnectionTcp::instance();
        while(connection_tcp->isRunning())
        {
            connection_tcp->stop();
            connection_tcp->wait();
        }

        closeCritical();
    }
    else
        closeNoCritical();

    hide();
}
