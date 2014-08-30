#ifndef FORMPLAYERINGAME_H
#define FORMPLAYERINGAME_H

#include "connectiontcp.h"

#include <QWidget>
#include <QIcon>

namespace Ui {
    class FormPlayerInGame;
}

class FormPlayerInGame : public QWidget
{
        Q_OBJECT

    public:

        explicit                FormPlayerInGame(QString nick, QString game, QChar team = 'G', bool editable = false, QWidget *parent = 0);

        ~FormPlayerInGame();

        QString                 getNick();

        void                    changeTeam(QChar team);

    private slots:

        void                    on__pb_exit_clicked();

        void                    on__pb_team_clicked();

    private:

        Ui::FormPlayerInGame*   _ui;

        QString                 _game;
};

#endif // FORMPLAYERINGAME_H
