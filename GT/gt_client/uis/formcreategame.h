#ifndef FORMCREATEGAME_H
#define FORMCREATEGAME_H

#include "form.h"
#include <QStringListModel>
#include <QPixmap>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QKeyEvent>

#include "terrainlistdelegate.h"
#include "gameglobals.h"
#include "connectiontcp.h"
#include "configuration.h"

namespace Ui {
    class FormCreateGame;
}

class FormCreateGame : public Form
{
        Q_OBJECT

    public:

        explicit            FormCreateGame(QWidget *parent = 0);

        ~FormCreateGame();

    public slots:

        void                show();

        void                hide();

    protected:

        bool                eventFilter(QObject *object, QEvent *event);

    private slots:

        void                preview(QModelIndex model_index);

        void                connectionClientConnected();

        void                on__pb_cancel_clicked();

        void                on__pb_accept_clicked();

    private:


        Ui::FormCreateGame* _ui;

        QStringListModel*   _model;

        int                 _index_of_current_terrain;

        int                 _counter_of_players;
};

#endif // FORMCREATEGAME_H
