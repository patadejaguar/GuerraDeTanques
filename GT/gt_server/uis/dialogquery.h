#ifndef DIALOGQUERY_H
#define DIALOGQUERY_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class DialogQuery;
}

class DialogQuery : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogQuery(QWidget *parent = 0);
    ~DialogQuery();

public slots:
    void exec(QSqlQueryModel *query_model);
    
private:
    Ui::DialogQuery *ui;
};

#endif // DIALOGQUERY_H
