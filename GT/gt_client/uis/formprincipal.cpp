#include "formprincipal.h"
#include "ui_formprincipal.h"
#include "forms.h"

FormPrincipal::FormPrincipal(QWidget *parent) :
    Form(parent),
    _ui(new Ui::FormPrincipal)
{
    _ui->setupUi(this);

    _form_beta = new FormBeta(parent);
    _form_beta->move(0, 80);

//    connect(ui->_pb_play,SIGNAL(clicked()),this,SIGNAL(play()));
//    connect(ui->_pb_options,SIGNAL(clicked()),this,SIGNAL(options()));
//    connect(ui->_pb_help,SIGNAL(clicked()),this,SIGNAL(help()));
//    connect(ui->_pb_credits,SIGNAL(clicked()),this,SIGNAL(credits()));
//    connect(ui->_pb_exit,SIGNAL(clicked()),this,SIGNAL(exit()));
}

FormPrincipal::~FormPrincipal()
{
    delete _ui;
}

void FormPrincipal::show()
{
    _form_beta->show();
    Form::show();
}

void FormPrincipal::hide()
{
    _form_beta->hide();
    Form::hide();
}

void FormPrincipal::on__pb_play_clicked()
{
    Forms* forms = Forms::instance();

    forms->_form_principal->disable();

    ConnectionTcp *connection_tcp = ConnectionTcp::instance();
    while(connection_tcp->isRunning())
    {
        connection_tcp->stop();
        connection_tcp->wait();
    }

    QString server = Configuration::networkServer();
    int port = Configuration::networkServerPort();

    connection_tcp->connectToHost(QHostAddress(server), port, Configuration::networkUseSsl());

    ((FormDialogMessage*)(forms->_form_message))->showInformation("Conexión", QString("Conectando al servidor %1 por el puerto %2.").arg(server).arg(port));
}

void FormPrincipal::on__pb_configuration_clicked()
{
    hide();
    Forms::instance()->_form_configuration->show();
}

void FormPrincipal::on__pb_help_clicked()
{
    hide();
    Forms::instance()->_form_help->show();
}

void FormPrincipal::on__pb_credits_clicked()
{
    hide();
    Forms::instance()->_form_credits->show();
}

void FormPrincipal::on__pb_exit_clicked()
{
    qApp->exit();
}

void FormPrincipal::on__pb_replays_clicked()
{
    hide();
    Forms::instance()->_form_replays_select->show();
}
