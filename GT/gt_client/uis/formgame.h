#ifndef FORMGAME_H
#define FORMGAME_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTcpSocket>
#include <QShortcut>
#include <QLabel>
#include <QGLWidget>

#include "tcpgameserver.h"
#include "tcpgameconnectionclient.h"
#include "gameglobals.h"
#include "form.h"
#include "utilities.h"
#include "forminitgame.h"
#include "randomnumbermannager.h"
#include "game.h"
#include "itank.h"
#include "terrain.h"
#include "iobject.h"
#include "fadelabeleffect.h"

namespace Ui {
    class FormGame;
}

class FormGame : public Form
{
        Q_OBJECT

    public:

        explicit FormGame(QWidget *parent = 0);

        ~FormGame();

    public slots:

        void                    hide();

        void                    show(QList<PlayerData> players_data, int index_of_terrain, QPixmap presentation);

        void                    prepareSceneAndView();

    protected:

        bool                    eventFilter(QObject *object, QEvent *event);

        void                    keyPressEvent(QKeyEvent *event);

        void                    keyReleaseEvent(QKeyEvent *event);

        void                    mousePressEvent(QMouseEvent* event);

        void                    mouseReleaseEvent(QMouseEvent* event);

        void                    processKeyEvent(int key, bool value);

    private slots:

        void                    terrainError(QString error);

        void                    terrainLoadStep(QString step);

        void                    terrainLoadFinished();

        void                    clientEndSync(qint64 server_latency);

        void                    startGameIn5Sec();

        void                    counterSec();

        void                    actionsChange(Actions actions);

        void                    pauseGame(int player_index);
        void                    resumeGame(int player_index);

        void                    playerLeaveGame(PlayerData pd);

        void                    updateCurrentLive(int value);
        void                    updateMaxLive(int value);
        void                    updateCurrentMana(int value);
        void                    updateMaxMana(int value);
        void                    updateEffect();
        void                    updateTankView();
        void                    updateExperience(int value, int max);
        void                    updateSkill_1Cooldown(int percent);
        void                    updateSkill_2Cooldown(int percent);
        void                    updateSkill_3Cooldown(int percent);

        void                    updateTankDeaths(ITank* killer);
        void                    updateTankLevel(int value);
        void                    updateTankTimeToReborn(int value);
        void                    updateTankReborn();

        void                    circleOfPowerDesactive(ITank *killer);

        void                    gameOver();

        void                    showResults(int team_win);

        void                    advance();

        void                    advanceFrame();

        void                    sendData();

        void                    startGame();

        void                    clientGameDisconnected();

        void                    updateSkill(int skill, int level);

        void                    updatePointsForUpdateSkills();

        void                    addLog(QString log);

        void                    timeControl();
        void                    showChatMessage();
        void                    showChatMessageForAll();

        void                    scapeShorcut();

        void                    on__pb_time_control_clicked();
        void                    on__pb_chat_clicked();
        void                    on__pb_sound_music_clicked(bool checked);
        void                    on__pb_close_clicked();
        void                    on__vs_sound_valueChanged(int value);
        void                    on__vs_music_valueChanged(int value);

        void                    closeGame();
        void                    cancelCloseGame();

    private:

        void                    createGame();

        void                    updateIntegerLabel(QLabel *label, int value, int index_negative_value = INT_MIN, QColor positive = Qt::white, QColor negative = Qt::red, bool positive_simbol = true);

        QPixmap                 getNoActiveSkill(QPixmap base);

        void                    showInformationSkill(QPoint pos);

        //------------------------------------------------------------
        Ui::FormGame*               _ui;

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

        int                         _index_of_terrain;

        ITank*                      _tank;
        QList<ITank*>               _tanks;

        int                         _green_circles_destroyers;
        int                         _red_circles_destroyers;

        QMap<int,int>               _match_players;

        QMap<int, bool>             _controls;
        QMap<int, bool>             _last_controls;

        TcpGameServer*              _tcp_game_server;
        TcpGameConnectionClient*    _tcp_game_client;

        int                         _seconds_to_start_game;

        int                         _timing;

        bool                        _send_data;

        QList<Actions>              _actions;

        //for send data
        QString                     _fs_message;
        bool                        _fs_for_all;
        bool                        _fs_pause_game;

        //shorcuts
        QShortcut*                  _short_cut_time_control;
        QShortcut*                  _short_cut_message;
        QShortcut*                  _short_cut_message_for_all;
        QShortcut*                  _short_cut_scape;

        QList<PlayerData>                                   _players_data;
        QMap< PlayerData, QMap< PlayerData, KillsDeaths > > _results;
        QList<PlayerInformation>    _players_information;

        QList<FadeLabelEffect*>*    _logs;

        int                         _frame_counter;

        QFile*                      _log_file;
};

#endif // FORMGAME_H
