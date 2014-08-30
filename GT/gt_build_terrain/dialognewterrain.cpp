#include "dialognewterrain.h"
#include "ui_dialognewterrain.h"

DialogNewTerrain::DialogNewTerrain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTerrain)
{
    ui->setupUi(this);
}

DialogNewTerrain::~DialogNewTerrain()
{
    delete ui;
}

void DialogNewTerrain::loadTools()
{
    QString url = QFileDialog::getOpenFileName(0,"Seleccione el fichero de terreno","","Texturas (*.tools)");
    ui->_l_tools->setText(url);
    QFile file(url);
    QXmlStreamReader stream(&file);
    if(file.open(QIODevice::ReadOnly))
    {
        Terrain *terrain = Terrain::instance();
        if(terrain->parseTools(stream))
        {
            ui->_l_tile_widht->setText(QString("%1 px").arg(QString::number(terrain->tileWidth())));
            ui->_l_tile_height->setText(QString("%1 px").arg(QString::number(terrain->tileHeight())));
            ui->_l_wpx->setText(QString("%1 px").arg(QString::number(terrain->tileWidth()*ui->_sb_width->value())));
            ui->_l_hpx->setText(QString("%1 px").arg(QString::number(terrain->tileHeight()*ui->_sb_height->value())));
        }
    }
}

void DialogNewTerrain::accept()
{
    Terrain *terrain = Terrain::instance();
    terrain->setProperties(ui->_le_name->text(), ui->_sb_width->value(), ui->_sb_height->value());
    QDialog::accept();
}

void DialogNewTerrain::on__sb_width_valueChanged(int arg1)
{
    ui->_l_wpx->setText(QString("%1 px").arg(QString::number(Terrain::instance()->tileWidth()*arg1)));
}

void DialogNewTerrain::on__sb_height_valueChanged(int arg1)
{
    ui->_l_hpx->setText(QString("%1 px").arg(QString::number(Terrain::instance()->tileHeight()*arg1)));
}
