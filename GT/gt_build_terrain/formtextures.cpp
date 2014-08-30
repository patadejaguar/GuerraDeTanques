#include "formtextures.h"
#include "ui_formtextures.h"

FormTextures::FormTextures(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTextures)
{
    ui->setupUi(this);

    _grid_layout_tools = new QGridLayout(ui->_f_tools);
    _group_tools = new QButtonGroup();

     connect(_group_tools,SIGNAL(buttonClicked(int)),SLOT(activateTool(int)));
    connect(ui->_sb_variation,SIGNAL(valueChanged(int)),SLOT(updateToolPreview(int)));
}

FormTextures::~FormTextures()
{
    delete ui;
}

void FormTextures::loadTerrainTools()
{
    int row = 0;
    int column = 0;
    int id = 0;

    QList<Tool*> tools = Terrain::instance()->terrainTools();

    foreach (Tool* tool, tools)
    {
        QPushButton *button = new QPushButton();
        button->setMinimumSize(40,40);
        button->setMaximumSize(40,40);
        button->setCheckable(true);
        button->setIconSize(QSize(32,32));
        QImage image = QImage(32,32,QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        QPixmap pixmap = tool->getTile(14);
        QPixmap obstruction = QPixmap(":/obstruction.png");
        painter.drawPixmap(0,0,pixmap.scaled(32,32));
        if(tool->obstruction())
            painter.drawPixmap(16,0,obstruction.scaled(16,16));
        painter.end();

        button->setIcon(QIcon(QPixmap::fromImage(image)));
        button->setToolTip(tool->name());
        _group_tools->addButton(button,id++);
        _grid_layout_tools->addWidget(button,row,column++);
        if(column == 4)
        {
            column = 0;
            row++;
        }
    }
    _group_tools->buttons().at(0)->click();
}

void FormTextures::reset()
{
    while(_group_tools->buttons().size() != 0)
    {
        QPushButton *button = (QPushButton*)_group_tools->buttons().at(0);
        _group_tools->removeButton(button);
        _grid_layout_tools->removeWidget(button);
        delete button;
    }

    ui->_sb_variation->setMaximum(0);
    ui->_sb_cursor->setValue(1);
    ui->_l_preview->setPixmap(QPixmap());
}

void FormTextures::activateTool(int id)
{
    Terrain::instance()->setActivateTool(id);
    int max = Terrain::instance()->activateTool()->getVariationsSize();
    ui->_sb_variation->setMaximum(max);
    updateToolPreview(ui->_sb_variation->value());
}

void FormTextures::updateToolPreview(int variation)
{
    QImage image(96,96,QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    Tool *tool = Terrain::instance()->activateTool();
    painter.drawPixmap(0,0,32,32,tool->getTile(0));
    painter.drawPixmap(0,32,32,32,tool->getTile(4));
    painter.drawPixmap(0,64,32,32,tool->getTile(3));
    painter.drawPixmap(32,0,32,32,tool->getTile(2));
    painter.drawPixmap(32,32,32,32,tool->getTile(14+variation));
    painter.drawPixmap(32,64,32,32,tool->getTile(11));
    painter.drawPixmap(64,0,32,32,tool->getTile(1));
    painter.drawPixmap(64,32,32,32,tool->getTile(9));
    painter.drawPixmap(64,64,32,32,tool->getTile(7));
    painter.end();
    ui->_l_preview->setPixmap(QPixmap::fromImage(image));
}

void FormTextures::on__sb_cursor_valueChanged(int arg1)
{
    Terrain::instance()->setCursorSize(arg1);
}

void FormTextures::on__sb_variation_valueChanged(int arg1)
{
    Terrain::instance()->setActivateRandom(arg1);
}

void FormTextures::on__cb_dibujar_aleatorio_clicked(bool checked)
{
    Terrain::instance()->setDrawRandom(checked);

}
