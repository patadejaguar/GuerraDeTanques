#ifndef FORMOBSTRUCTIONS_H
#define FORMOBSTRUCTIONS_H

#include <QWidget>

#include "terrain.h"
#include "scene.h"

namespace Ui {
    class FormObstructions;
}

class FormObstructions : public QWidget
{
        Q_OBJECT

    public:
        explicit FormObstructions(Scene* scene, QWidget *parent = 0);
        ~FormObstructions();

    private slots:
        void on__pb_rectangle_clicked();

        void on__pb_polygon_clicked();

    private:
        Ui::FormObstructions *ui;
        Scene* _scene;
};

#endif // FORMOBSTRUCTIONS_H
