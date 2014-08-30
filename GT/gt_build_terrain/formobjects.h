#ifndef FORMOBJECTS_H
#define FORMOBJECTS_H

#include <QWidget>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPushButton>

#include "terrain.h"
#include "scene.h"

namespace Ui {
    class FormObjects;
}

class FormObjects : public QWidget
{
        Q_OBJECT

    public:
        explicit FormObjects(Scene *scene, QWidget *parent = 0);
        ~FormObjects();

    private slots:
        void updateObjectProperties(QList<QWidget *> controls);

        void clearObjectProperties();
        void activateObject(int id);

        void on__pb_cursor_clicked();

        void on__pb_snap_to_grid_clicked();

    private:
        void reset();
        void loadObjects();

        Ui::FormObjects *ui;
        Scene *_scene;

        QGridLayout *_grid_layout_objects;
        QButtonGroup *_group_objects;

        QWidget *_widget_properties;
        QVBoxLayout *_layout_properties;
};

#endif // FORMOBJECTS_H
