#include "forminitgame.h"
#include "ui_forminitgame.h"

FormInitGame::FormInitGame(QWidget *parent) :
    Form(parent),
    ui(new Ui::FormInitGame)
{
    ui->setupUi(this);
}

FormInitGame::~FormInitGame()
{
    delete ui;
}

void FormInitGame::setPixmap(QPixmap pixmap)
{
    ui->_l_image->setPixmap(pixmap);
}

void FormInitGame::setText(QString text)
{
    ui->_l_text->setText(text);
}
