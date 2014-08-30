#ifndef FORMREPLAYVIEW_H
#define FORMREPLAYVIEW_H

#include "form.h"

#include <QGraphicsScene>
#include <QShortcut>
#include <QLabel>

#include "gameglobals.h"
#include "utilities.h"
#include "forminitgame.h"
#include "randomnumbermannager.h"
#include "game.h"
#include "itank.h"
#include "terrain.h"
#include "iobject.h"
#include "fadelabeleffect.h"

namespace Ui {
    class FormReplayView;
}

class FormReplayView : public Form
{
        Q_OBJECT

    public:

        explicit FormReplayView(QWidget *parent = 0);
        ~FormReplayView();

    public slots:

        void                        show(QString replay_file);

    protected:

        bool                        eventFilter(QObject *object, QEvent *event);

    private slots:

        void                        timeControl();

        void                        terrainError(QString error);
        void                        terrainLoadStep(QString step);
        void                        terrainLoadFinished();

        void                        advance();
        void                        advanceFrame();

        void                        addLog(QString log);

        void                        startGame();

        void                        pauseGame(int player_index);
        void                        resumeGame(int player_index);

        void                        prepareSceneAndView();

        void                        createGame(QList<PlayerInformation> players_information);

        void                        updateCurrentLive(int value);
        void                        updateMaxLive(int value);
        void                        updateCurrentMana(int value);
        void                        updateMaxMana(int value);
        void                        updateEffect();
        void                        updateExperience(int value, int max);
        void                        updateSkill_1Cooldown(int percent);
        void                        updateSkill_2Cooldown(int percent);
        void                        updateSkill_3Cooldown(int percent);

        void                        updateTankReference(int index_of_tank);
        void                        updateTankView();

        QPixmap                     getNoActiveSkill(QPixmap base);

        void                        updateIntegerLabel(QLabel *label, int value, int index_negative_value = INT_MIN, QColor positive = Qt::white, QColor negative = Qt::red, bool positive_symbol = true);
        void                        updateSkill(int skill, int level);
        void                        updatePointsForUpdateSkills();

        void                        updateTankDeaths(ITank *killer);
        void                        updateTankLevel(int value);
        void                        updateTankTimeToReborn(int value);
        void                        updateTankReborn();
        void                        circleOfPowerDesactive(ITank *killer);

        void                        on__pb_close_clicked();
        void                        on__pb_time_control_clicked();
        void                        on__pb_sound_music_clicked(bool checked);
        void                        on__vs_sound_valueChanged(int value);
        void                        on__vs_music_valueChanged(int value);
        void                        on__cb_players_activated(int index);

    private:

        Ui::FormReplayView *        _ui;

        QString                     _text_style_template;

        QPixmap                     _preview_skill_1;
        QPixmap                     _preview_skill_2;
        QPixmap                     _preview_skill_3;

        QPixmap                     _no_active_skill_1;
        QPixmap                     _no_active_skill_2;
        QPixmap                     _no_active_skill_3;

        QPixmap                     _skill_locked;

        QList<QPixmap>              _skills_cooldown;

        QGraphicsScene*             _scene;
        QGraphicsScene*             _mini_map_scene;

        QTimer*                     _timer;

        Game*                       _game;
        Team*                       _team_1;
        Team*                       _team_2;

        int                         _index_of_terrain;

        ITank*                      _tank;
        QList<ITank*>               _tanks;

        int                         _green_circles_destroyers;
        int                         _red_circles_destroyers;

        QMap<int,int>               _match_players;

        QMap<int, bool>             _controls;
        QMap<int, bool>             _last_controls;

        int                         _timing;

        QList<Actions>              _actions;

        int                         _total_of_actions;

        QList<PlayerInformation>    _players_information;

        QShortcut*                  _short_cut_time_control;

        QList<FadeLabelEffect*>*    _logs;

        int                         _frame_counter;
};

#endif // FORMREPLAYVIEW_H
