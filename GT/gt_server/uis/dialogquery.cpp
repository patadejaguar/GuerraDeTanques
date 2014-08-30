#include "dialogquery.h"
#include "ui_dialogquery.h"

DialogQuery::DialogQuery(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQuery)
{
    ui->setupUi(this);
}

DialogQuery::~DialogQuery()
{
    delete ui;
}

void DialogQuery::exec(QSqlQueryModel *query_model)
{
    if(query_model->rowCount() == 0)
        return;
    ui->tableView->setModel(query_model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    QDialog::exec();
}
