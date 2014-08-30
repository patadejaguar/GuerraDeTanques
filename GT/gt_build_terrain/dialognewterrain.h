#ifndef DIALOGNEWTERRAIN_H
#define DIALOGNEWTERRAIN_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QMessageBox>
#include "terrain.h"

namespace Ui {
class DialogNewTerrain;
}

class DialogNewTerrain : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewTerrain(QWidget *parent = 0);
    ~DialogNewTerrain();

private slots:
    void loadTools();
    void accept();
    void on__sb_width_valueChanged(int arg1);
    void on__sb_height_valueChanged(int arg1);

private:
    Ui::DialogNewTerrain *ui;
};

#endif // DIALOGNEWTERRAIN_H
