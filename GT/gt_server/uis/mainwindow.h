#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QVBoxLayout>
#include "serverconfiguration.h"
#include "databaseconnection.h"
#include "server.h"
#include "connection.h"
#include "dialogconfiguration.h"
#include "connectionmodel.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit                MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void                    resizeEvent(QResizeEvent *);

    public slots:

        void                    dataBaseError(QString error);
        void                    tcpServerError(QString error);

    private slots:

        void                    createListOfConnections();
        void                    saveSettings();
        void                    readSettings();

        void                    loadDataBase();
        void                    createTcpServer();

        void                    tcpServerListening();

        void                    newConnection(TcpConnectionServer *connection);

        void                    on_actionConfiguraci_n_triggered();
        void                    on__pb_execute_query_clicked();
        void                    on_actionHabilitar_Servidor_triggered();
        void                    on__pb_update_tables_clicked();
        void                    on_actionSalir_triggered();
        void                    on__le_sql_query_textEdited(const QString &arg1);

    protected:
        void                    closeEvent(QCloseEvent *event);

    private:

        Ui::MainWindow*         _ui;

        Server*                 _server;
        ConnectionModel*        _connection_model;

        DataBaseConnection*     _data_base_connection;
        QSqlTableModel*         _table_user_model;
};

#endif // MAINWINDOW_H
