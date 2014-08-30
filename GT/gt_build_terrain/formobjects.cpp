#include "formobjects.h"
#include "ui_formobjects.h"

FormObjects::FormObjects(Scene *scene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormObjects)
{
    ui->setupUi(this);
    _scene = scene;

    _grid_layout_objects = new QGridLayout(ui->_f_objects);
    _group_objects = new QButtonGroup();

    connect(_group_objects,SIGNAL(buttonClicked(int)),SLOT(activateObject(int)));
    connect(_scene, SIGNAL(updateObjectProperties(QList<QWidget*>)), this,SLOT(updateObjectProperties(QList<QWidget*>)));
    connect(_scene,SIGNAL(noObjectSelected()),this,SLOT(clearObjectProperties()));

    _widget_properties = NULL;
    _layout_properties = NULL;

    loadObjects();
}

FormObjects::~FormObjects()
{
    delete ui;
}

void FormObjects::activateObject(int id)
{
    if(id == 0)
        return;
    id--;

    _scene->setActivateCursor(false);
    Terrain::instance()->setActivateObjectFactory(id);
    _scene->updateObjectPreview();
}

void FormObjects::reset()
{

}

void FormObjects::loadObjects()
{
    int row = 0;
    int column = 0;
    int id = 0;

    Terrain::instance()->loadObjectFactories();

    _group_objects->addButton(ui->_pb_cursor,id++);
    foreach (IObjectFactory* object, Terrain::instance()->objectFactories())
    {
        QPushButton *button = new QPushButton();
        button->setMinimumSize(40,40);
        button->setMaximumSize(40,40);
        button->setCheckable(true);
        button->setIconSize(QSize(32,32));
        QPixmap pixmap = object->getPreview();
        button->setIcon(QIcon(pixmap));
        button->setToolTip(object->pluginName());
        _group_objects->addButton(button,id++);
        _grid_layout_objects->addWidget(button,row,column++);
        if(column == 4)
        {
            column = 0;
            row++;
        }
    }
    _group_objects->buttons().at(0)->click();
}

void FormObjects::updateObjectProperties(QList<QWidget *> controls)
{
    if(_widget_properties  != NULL)
        delete _widget_properties;

    _widget_properties = new QWidget();
    _layout_properties = new QVBoxLayout(_widget_properties);

    foreach(QWidget* w, controls)
    {
        _layout_properties->addWidget(w);
    }
    _layout_properties->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Minimum,QSizePolicy::Expanding));
    ui->_sa_properties->setWidget(_widget_properties);
}

void FormObjects::clearObjectProperties()
{
    if(_widget_properties  != NULL)
        delete _widget_properties;

    _widget_properties = new QWidget();
    ui->_sa_properties->setWidget(_widget_properties);
}


void FormObjects::on__pb_cursor_clicked()
{
    _scene->setActivateCursor(ui->_pb_cursor->isChecked());
}

void FormObjects::on__pb_snap_to_grid_clicked()
{
    _scene->setSnapToGrid(ui->_pb_snap_to_grid->isChecked());
}
