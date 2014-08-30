#include "formobstructions.h"
#include "ui_formobstructions.h"

FormObstructions::FormObstructions(Scene* scene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormObstructions)
{
    ui->setupUi(this);
    _scene = scene;
}

FormObstructions::~FormObstructions()
{
    delete ui;
}

void FormObstructions::on__pb_rectangle_clicked()
{
//    _scene->setTool_obstruction(TOOL_RECTANGLE);
}

void FormObstructions::on__pb_polygon_clicked()
{
//    _scene->setTool_obstruction(TOOL_RECTANGLE);
}
