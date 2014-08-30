#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QPixmap>
#include <QBitmap>
#include <QBuffer>
#include <QTimer>
#include <QGLWidget>

#include "scene.h"
#include "dialognewterrain.h"
#include "grid.h"

#include "view.h"
#include "treearea.h"

#include "formtextures.h"
#include "formobjects.h"
#include "formobstructions.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void on_actionNuevo_triggered();
        void on_actionGuardar_triggered();
        void on_actionAbrir_triggered();
        void on_actionGenerar_terreno_triggered();
        void on_actionGuardar_como_Imagen_jpg_triggered();

        void addElementsToScene();

        void on_actionCuadr_cula_triggered();

        void on_actionZoom_100_triggered();

        void saveWidget();
        void on__pb_layer_terrain_clicked();
        void on__pb_layer_objects_clicked();
        void on__pb_layer_obstructions_clicked();

        void on__pb_load_image_clicked();

        void on__le_name_textChanged(const QString &arg1);

    private:

        void reset();

        Ui::MainWindow *ui;

        View *_view;
        Scene *_scene;

        Grid *_grid;

        QTimer* _timer;

        FormTextures* _form_textures;
        FormObjects* _form_objects;
        FormObstructions* _form_obstructions;
};

#endif // MAINWINDOW_H
