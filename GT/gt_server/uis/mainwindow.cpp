#include <QSqlError>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogquery.h"
#include "avatardelegate.h"
#include "passworddelegate.h"
#include "connectiondelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    qRegisterMetaType< UserProperties >("UserProperties");
    qRegisterMetaType< QList<UserProperties> >("QList<UserProperties>");
    qRegisterMetaType< KillsDeaths >("KillsDeaths");
    qRegisterMetaType< QMap< PlayerData, QMap< PlayerData, KillsDeaths > > >("QMap< PlayerData, QMap< PlayerData, KillsDeaths > >");

    createListOfConnections();

    readSettings();
    loadDataBase();
    createTcpServer();
}

MainWindow::~MainWindow()
{
    delete _ui;
    delete _server;
    delete _table_user_model;
    delete _connection_model;
    delete _data_base_connection;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    _ui->_tv_player->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::dataBaseError(QString error)
{
    QMessageBox::critical(this,tr("Error de Base de Datos"), error);
    qApp->exit(1);
}

void MainWindow::tcpServerError(QString error)
{
    QMessageBox::critical(this,tr("Error de Servidor TCP"), error);
    _ui->statusbar->showMessage(tr("Error en servidor: ") + error);
}

void MainWindow::createListOfConnections()
{
    _connection_model = new ConnectionModel();
    _ui->_lv_connections->setModel(_connection_model);
    ConnectionDelegate *connection_delegate = new ConnectionDelegate(this);
    _ui->_lv_connections->setItemDelegate(connection_delegate);
}

void MainWindow::loadDataBase()
{
    _data_base_connection = ServerConfiguration::getNewDataBaseConnection();
    _data_base_connection->open();

    connect(_data_base_connection,SIGNAL(error(QString)),this,SLOT(dataBaseError(QString)));

    _data_base_connection->executeQuery("UPDATE Player SET connected = \"false\";");

    _table_user_model = new QSqlTableModel(this, _data_base_connection->getDataBase());
    _table_user_model->setTable("Player");
    _table_user_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    _table_user_model->select();
    _ui->_tv_player->setModel(_table_user_model);
    _ui->_tv_player->horizontalHeader()->setStretchLastSection(true);

    AvatarDelegate *avatar_delegate = new AvatarDelegate(this);
    _ui->_tv_player->setItemDelegateForColumn(3, avatar_delegate);

    PasswordDelegate *password_delegate = new PasswordDelegate(this);
    _ui->_tv_player->setItemDelegateForColumn(2, password_delegate);
}

void MainWindow::createTcpServer()
{
    _server = new Server(ServerConfiguration::instance()->useSsl());
    connect(_server, &Server::notifyError, this, &MainWindow::tcpServerError);
    connect(_server, &Server::notifyNewConnection, this, &MainWindow::newConnection);
    on_actionHabilitar_Servidor_triggered();
}

void MainWindow::tcpServerListening()
{
    _ui->statusbar->showMessage(tr("Servidor encendido"));
}

void MainWindow::newConnection(TcpConnectionServer *connection)
{
    _connection_model->addConnection((Connection*)connection);
}

void MainWindow::saveSettings()
{
    ServerConfiguration *configuration = ServerConfiguration::instance();
    QSettings settings("server_config", QSettings::IniFormat);
    settings.setValue("configure", true);
    settings.setValue("geometry", geometry());
    settings.setValue("is_maximized",isMaximized());
    settings.setValue("use_ssl",configuration->useSsl());
    settings.setValue("port",configuration->getPort());
}

void MainWindow::readSettings()
{
    ServerConfiguration *configuration = ServerConfiguration::instance();
    QSettings settings("server_config", QSettings::IniFormat);
    if(!settings.value("configure").toBool())
    {
        DialogConfiguration *dialog = new DialogConfiguration(this);
        dialog->exec();
        delete dialog;
        return;
    }
    bool maximized = settings.value("is_maximized").toBool();
    if(maximized)
        setWindowState(Qt::WindowMaximized);
    else
        setGeometry(settings.value("geometry").toRect());

    configuration->setUseSsl(settings.value("use_ssl", false).toBool());
    configuration->setPort(settings.value("port", 9090).toInt());
}

void MainWindow::on_actionConfiguraci_n_triggered()
{
    DialogConfiguration *dialog = new DialogConfiguration(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on__pb_execute_query_clicked()
{
    QMessageBox message_box(QMessageBox::Warning,tr("GT Servidor"),tr("¿Seguro que desea ejecutar la consulta?"));
    message_box.addButton("Si",QMessageBox::YesRole);
    message_box.addButton("No",QMessageBox::NoRole);
    if(message_box.exec() == 1)
        return;

    QSqlQueryModel *query_model = new QSqlQueryModel;
    query_model->setQuery(_ui->_le_sql_query->text());
    if(query_model->lastError().isValid())
    {
        QMessageBox::critical(this,tr("Error en la consulta"), query_model->lastError().text());
        delete query_model;
        return;
    }
    DialogQuery *dialog = new DialogQuery(this);
    dialog->exec(query_model);
    delete dialog;
    delete query_model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox message_box(QMessageBox::Warning,tr("GT Servidor"),tr("¿Seguro que desea salir?"));
    message_box.addButton("Si",QMessageBox::YesRole);
    message_box.addButton("No",QMessageBox::NoRole);
    if(message_box.exec() == 0)
    {
        saveSettings();
        _server->close();
        QMainWindow::closeEvent(event);
    }
    else
        event->ignore();
}

void MainWindow::on_actionHabilitar_Servidor_triggered()
{
    if(_ui->actionHabilitar_Servidor->isChecked())
    {
        _data_base_connection->executeQuery("UPDATE User SET connected = \"false\";");

        ServerConfiguration *configuration = ServerConfiguration::instance();
        QString security;
        configuration->useSsl()?security = "Seguro":security = "Inseguro";
        if(_server->listen(QHostAddress::Any,configuration->getPort()))
            _ui->statusbar->showMessage(QString(tr("Servidor encendido: puerto[%1] [%2]")).arg(configuration->getPort()).arg(security));
        else
            tcpServerError(_server->errorString());
    }
    else
    {
        _server->close();
        _ui->statusbar->showMessage(tr("Servidor apagado"));
    }
}

void MainWindow::on__pb_update_tables_clicked()
{
    _table_user_model->select();
}

void MainWindow::on_actionSalir_triggered()
{
    close();
}

void MainWindow::on__le_sql_query_textEdited(const QString &arg1)
{
    _ui->_pb_execute_query->setEnabled(arg1 != "");
}
