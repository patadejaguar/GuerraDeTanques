#ifndef SERVERSTATUS_H
#define SERVERSTATUS_H

#include <QWidget>

namespace Ui {
class ServerStatus;
}

class ServerStatus : public QWidget
{
    Q_OBJECT
    
public:
    explicit ServerStatus(QWidget *parent = 0);
    ~ServerStatus();
    
private:
    Ui::ServerStatus *ui;
};

#endif // SERVERSTATUS_H
