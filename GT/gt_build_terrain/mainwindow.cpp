#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _view = new View();
    ui->gridLayout->addWidget(_view, 0, 0, 1, 1);

    _scene = new Scene(_view);
    _scene->setBackgroundBrush(Qt::black);
    _view->setScene(_scene);

//    QGLFormat gl_format(QGL::Rgba | QGL::SampleBuffers | QGL::DirectRendering | QGL::AlphaChannel);
//    _view->setViewport(new QGLWidget(gl_format, this));

    ui->_gv_mini_map->setScene(_scene);

    _timer = new QTimer();
    QObject::connect(_timer, SIGNAL(timeout()), _scene, SLOT(advance()));
//    _timer->start(50);

    _form_textures = new FormTextures();
    _form_objects = new FormObjects(_scene);
    _form_obstructions = new FormObstructions(_scene);

    ui->_sa_tools->setWidget(_form_textures);

    connect(ui->_sb_zoom,SIGNAL(valueChanged(int)),_view,SLOT(setZoom(int)));
    connect(_view,SIGNAL(zoomChange(int)),ui->_sb_zoom,SLOT(setValue(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete _scene;
    delete _view;
    delete _form_textures;
    delete _form_objects;
    delete _form_obstructions;
}

void MainWindow::on_actionNuevo_triggered()
{
    reset();
    DialogNewTerrain *wizard_new_terrain = new DialogNewTerrain(this);
    if(wizard_new_terrain->exec() == QDialog::Accepted)
    {
        Terrain *terrain = Terrain::instance();
        terrain->newTerrain();
        terrain->setActivateTool(Terrain::instance()->terrainTools().size()-1);
        _scene->setSceneRect(0, 0, terrain->widthInPx(), terrain->heightInPx());

        ui->_gv_mini_map->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);

        addElementsToScene();

        _form_textures->loadTerrainTools();
    }
    delete wizard_new_terrain;
    ui->_le_name->setText(Terrain::instance()->name());
}

void MainWindow::on_actionCuadr_cula_triggered()
{
    _grid->setVisible(ui->actionCuadr_cula->isChecked());
}


void MainWindow::reset()
{
    Terrain *terrain = Terrain::instance();
    terrain->reset();

    _scene->clear();

    _grid = new Grid();
    _grid->setZValue(1);
    terrain->addObserver(_grid);
    _scene->addItem(_grid);

    ui->_sb_zoom->setValue(100);

    _form_textures->reset();
}

void MainWindow::on_actionZoom_100_triggered()
{
    ui->_sb_zoom->setValue(100);
}

void MainWindow::saveWidget()
{
    Terrain* terrain = Terrain::instance();

    if(terrain->currentLayer() == L_TERRAIN)
        _form_textures = dynamic_cast<FormTextures*>(ui->_sa_tools->takeWidget());
    else if(terrain->currentLayer() == L_OBJECT)
        _form_objects = dynamic_cast<FormObjects*>(ui->_sa_tools->takeWidget());
    else if(terrain->currentLayer() == L_OBSTRUCTIONS)
        _form_obstructions = dynamic_cast<FormObstructions*>(ui->_sa_tools->takeWidget());
}

void MainWindow::on_actionGuardar_como_Imagen_jpg_triggered()
{
    QString file = QFileDialog::getSaveFileName(this,"Guardar como imagen","image","Imagen (*.jpg)");
    if(file != "")
    {
        if(!file.endsWith(".jpg",Qt::CaseInsensitive))
            file+=".jpg";

        QImage image = QImage((int)_scene->width(),(int)_scene->height(),QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&image);
        _scene->setVisiblePreview(false);
        _grid->setVisible(false);
        _scene->render(&painter);
        _scene->setVisiblePreview(true);
        _grid->setVisible(true);
        image.save(file,"JPG",100);
    }
}

void MainWindow::addElementsToScene()
{
    Terrain *terrain = Terrain::instance();
    foreach (QList<Tile*>* row_tiles, terrain->tiles())
        foreach (Tile* tile, *row_tiles)
            _scene->addItem(tile);

    foreach (QList<DrawPoint*>* row_draw_points, terrain->drawPoints())
        foreach (DrawPoint* draw_point, *row_draw_points)
            _scene->addItem(draw_point);

    foreach (IObject* object, terrain->objects())
        _scene->addItem(object);
}

void MainWindow::on_actionGuardar_triggered()
{
    QString url = QFileDialog::getExistingDirectory(this,"Guardar terreno","");
    if(url != "")
    {
        Terrain *terrain = Terrain::instance();

        QFile file(url+"\\"+terrain->name()+".ter");
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            stream << *terrain;
            file.close();
        }
        else
            QMessageBox::critical(this,"Error","Imposible guardar el fichero.");
    }
}

void MainWindow::on_actionAbrir_triggered()
{
    reset();
    QString url = QFileDialog::getOpenFileName(this,"Abrir fichero ter","","Terreno (*.ter)");
    if(url != "")
    {
        QFile file(url);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream stream(&file);
            Terrain *terrain = Terrain::instance();
            stream >> *terrain;

            ui->_le_name->setText(terrain->name());

            _scene->setSceneRect(0, 0, terrain->widthInPx(), terrain->heightInPx());
            addElementsToScene();

            ui->_gv_mini_map->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);

            _form_textures->loadTerrainTools();
        }
        else
            QMessageBox::critical(this,"Error","Imposible abrir el fichero.");
    }
}

void MainWindow::on_actionGenerar_terreno_triggered()
{
    Terrain *terrain = Terrain::instance();

    int counter_green = 0;
    int order_green = 0;
    int counter_red = 0;
    int order_red = 0;

    QList<int> players;
    QString error = "";
    foreach(IObject* obj, terrain->objects())
    {
        CircleOfPower* cp = dynamic_cast<CircleOfPower*>(obj);
        if(cp)
        {
            if(cp->team() == 1)
            {
                ++counter_green;
                order_green += cp->player();
            }
            else
            {
                ++counter_red;
                order_red += cp->player();
            }
            int player = (cp->team()-1)*3+cp->player();
            if(!players.contains(player))
                players.append(player);
            else
                error+=QString("El jugador %1 del equipo %2 está duplicado.\n").arg(cp->player()).arg(cp->team());
        }
    }

    if(counter_green == 0)
        error+= QString("El equipo verde no tiene círculos de poder.\n");
    else if((counter_green == 1 && order_green != 1) || (counter_green == 2 && order_green !=3) || (counter_green == 3 && order_green != 6))
        error+= QString("Los jugadores del equipo verde no están ordenados correctamente.\n");
    if(counter_red == 0)
        error+= QString("El equipo rojo no tiene círculos de poder.\n");
    else if((counter_red == 1 && order_red != 1) || (counter_red == 2 && order_red !=3) || (counter_red == 3 && order_red != 6))
        error+= QString("Los jugadores del equipo rojo no están ordenados correctamente.\n");

    if(error != "")
    {
        QMessageBox::critical(this,"Error",error);
        return;
    }

    terrain->setCountersPlayers(counter_green, counter_red);

    QString url = QFileDialog::getExistingDirectory(this,"Generar terreno para juego","");
    if(url != "")
    {
        QFile file(url+"\\"+terrain->name()+".gmm");
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream stream(&file);
            terrain->generate(stream);
            file.close();
        }
        else
            QMessageBox::critical(this,"Error","Imposible guardar el fichero.");
    }
}

void MainWindow::on__pb_layer_terrain_clicked()
{
    saveWidget();
    ui->_sa_tools->setWidget(_form_textures);
    Terrain::instance()->setCurrentLayer(L_TERRAIN);
}

void MainWindow::on__pb_layer_objects_clicked()
{
    saveWidget();
    ui->_sa_tools->setWidget(_form_objects);
    Terrain::instance()->setCurrentLayer(L_OBJECT);
}

void MainWindow::on__pb_layer_obstructions_clicked()
{
    saveWidget();
    ui->_sa_tools->setWidget(_form_obstructions);
    Terrain::instance()->setCurrentLayer(L_OBSTRUCTIONS);
}

void MainWindow::on__pb_load_image_clicked()
{
    QString image = QFileDialog::getOpenFileName(0,"Cargar portada del terreno","","Imagen (*.jpg)");
    if(!image.isNull())
    {
        QPixmap pixmap;
        if(pixmap.load(image,"JPG"))
        {
            Terrain::instance()->setImage(pixmap.toImage());
            ui->_l_image_name->setText(image);
            ui->_l_image->setPixmap(pixmap);
        }
    }
}

void MainWindow::on__le_name_textChanged(const QString &arg1)
{
    Terrain::instance()->setName(arg1);
}

