#ifndef FORMTEXTURES_H
#define FORMTEXTURES_H

#include <QWidget>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPushButton>

#include "terrain.h"

namespace Ui {
    class FormTextures;
}

class FormTextures : public QWidget
{
        Q_OBJECT

    public:
        explicit FormTextures(QWidget *parent = 0);
        ~FormTextures();

        void reset();
        void loadTerrainTools();

    private slots:

        void activateTool(int id);
        void updateToolPreview(int variation);

        void on__sb_cursor_valueChanged(int arg1);

        void on__sb_variation_valueChanged(int arg1);

        void on__cb_dibujar_aleatorio_clicked(bool checked);

    private:
        Ui::FormTextures *ui;
        QGridLayout *_grid_layout_tools;
        QButtonGroup *_group_tools;
};

#endif // FORMTEXTURES_H
