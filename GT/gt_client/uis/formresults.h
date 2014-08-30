#ifndef FORMRESULTS_H
#define FORMRESULTS_H

#include "form.h"
#include "gameglobals.h"
#include "formgame.h"

namespace Ui {
    class FormResults;
}

class FormResults : public Form
{
        Q_OBJECT

    public:

        explicit FormResults(QWidget *parent = 0);
        ~FormResults();

    public slots:

        void                    show(int team_win, QList<PlayerInformation> players_information, QMap< PlayerData, QMap< PlayerData, KillsDeaths > > result, TerrainInfo terrain_info, QList<Actions> actions);

    private slots:

        void                    on__pb_chat_clicked();

        void                    on__pb_save_replay_clicked();

    private:

        Ui::FormResults*        _ui;

        TerrainInfo                 _terrain_info;
        QList<PlayerInformation>    _players_information;
        QList<Actions>              _actions;

};

#endif // FORMRESULTS_H
