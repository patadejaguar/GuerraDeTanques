#ifndef FORMCREATEDGAME_H
#define FORMCREATEDGAME_H

#include <QKeyEvent>
#include <QFile>
#include <QDir>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "form.h"
#include "connectiontcp.h"
#include "configuration.h"
#include "formplayeringame.h"
#include "gameglobals.h"
#include "tcpgameserver.h"
#include "tcpgameconnectionclient.h"
#include "resourcemanager.h"

enum SlotMode {SLOT_SERVER_MODE, SLOT_CLIENT_MODE};

struct Slot{

        QLabel*         _l_avatar;
        QLabel*         _l_rank;
        QLabel*         _l_nick;
        QLabel*         _l_tank;
        QLabel*         _l_team;
        QPushButton*    _pb_team;
        QLabel*         _l_latency;
        QPushButton*    _pb_close;

        SlotMode        _slot_mode;
        bool            _my;
        bool            _visible;
        bool            _ready;

        void            init(){
            _slot_mode = SLOT_CLIENT_MODE;
            _my = false;
            _visible = true;
            _ready = false;
        }

        void            setReady(bool value){            
            _ready = value;
            update();
        }

        void            setSlotMode(SlotMode slot_mode){
            _slot_mode = slot_mode;
            update();
        }

        void            setMy(bool value){
            _my = value;
            update();
        }

        void            setVisible(bool value){
            _visible = value;
            update();
        }

        void            update(){
            if(_visible)
            {
                show();
                if(_slot_mode == SLOT_SERVER_MODE)
                {
                    _pb_close->setEnabled(true);
                }
                else
                {
                    _pb_close->setEnabled(_my);
                }
                if(_ready)
                {
                    _l_team->setVisible(_ready);
                    _pb_team->setVisible(!_ready);
                    if(_slot_mode == SLOT_CLIENT_MODE)
                        _pb_close->setEnabled(!_ready);
                }
            }
            else
            {
                hide();
            }
        }

        void            show(){
            _l_rank->setVisible(true);
            _l_avatar->setVisible(true);
            _l_nick->setVisible(true);
            _ready ? _l_nick->setStyleSheet("color:"+COLOR_GREEN+";") : _l_nick->setStyleSheet("color:"+COLOR_WHITE+";");
            _l_tank->setVisible(true);
            if(_my)
            {
                _l_team->setVisible(false);
                _pb_team->setVisible(true);
            }
            else
            {
                _l_team->setVisible(true);
                _pb_team->setVisible(false);
            }
            _l_latency->setVisible(true);
            _pb_close->setVisible(true);
        }

        void            hide(){
            _l_rank->setVisible(false);
            _l_avatar->setVisible(false);
            _l_nick->setVisible(false);
            _l_tank->setVisible(false);
            _l_team->setVisible(false);
            _pb_team->setVisible(false);
            _l_latency->setVisible(false);
            _pb_close->setVisible(false);
        }

        void            setNick(QString nick){
            _l_nick->setText(nick);
        }

        void            setAvatar(QPixmap avatar){
            _l_avatar->setPixmap(avatar);
        }

        void            setTank(int index_of_tank){
            _l_tank->setPixmap(index_of_tank == -1 ? QPixmap(":/icons/random_tank.png") : ResourceManager::instance()->tankFactories().at(index_of_tank)->tankInfo()._preview);
        }

        void            setRank(int rank){
            _l_rank->setPixmap(QPixmap(QString(":/gt/ranks/rank_%1.png").arg(rank)));
        }

        void            setTeam(int team){
            QString t;
            team == 1 ? t = "green_team.png" : t = "red_team.png";
            _pb_team->setIcon(QIcon(QPixmap(QString(":/slots/%1").arg(t))));
            _l_team->setPixmap(QPixmap(QString(":/slots/%1").arg(t)).scaled(24,24,Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        void            setLatency(int latency){
            _l_latency->setText(QString::number(latency));
            if(latency >= 150)
                _l_latency->setStyleSheet("color: "+COLOR_RED+";");
            else if(latency >= 100)
                _l_latency->setStyleSheet("color: "+COLOR_ORANGE+";");
            else
                _l_latency->setStyleSheet("color: "+COLOR_WHITE+";");
        }

        void            clear()
        {
            init();
            update();
            _l_rank->setPixmap(QPixmap(":/gt/ranks/rank_1.png"));
            _l_avatar->setPixmap(QPixmap(":/slots/default_avatar.png"));
            _l_nick->clear();
            _l_nick->setStyleSheet("color: "+COLOR_WHITE+";");
            _l_tank->setPixmap(QPixmap(":/slots/default_avatar.png"));
            _l_team->setPixmap(QPixmap());
            _pb_team->setIcon(QIcon());
            _l_latency->setText("-");
            _l_latency->setStyleSheet("color: "+COLOR_WHITE+";");
        }
};

namespace Ui {
    class FormCreatedGame;
}

class FormCreatedGame : public Form
{
        Q_OBJECT

    public:

        explicit                    FormCreatedGame(QWidget *parent = 0);

                                    ~FormCreatedGame();

    public slots:

        void                        show();

        void                        hide();

    protected:

        bool                        eventFilter(QObject *object, QEvent *event);

    private slots:

        void                        playerJoinedSuccessful(int index_of_terrain, QList<PlayerData> players_data, QList<int> random_numbers);

        void                        newPlayerJoinedGame(PlayerData player_data);

        void                        playerLeaveGame(PlayerData player_data);

        void                        playersLatency(QMap<int, int> players_latency);

        void                        showMessage(QString message);

        void                        clientGameDisconnected();

        void                        closeGame(QString creator, QPixmap avatar, int rank);

        void                        changeTeam(int index, int team);

        void                        changeTank(int index, int index_of_tank);

        void                        changePlayerReady(int index, bool value);

        void                        prepareGame();

        void                        cancel();

        void                        on__pb_select_tank_clicked();

        void                        on__le_message_returnPressed();

        void                        on__pb_cancel_clicked();

        void                        on__pb_close_slot_1_clicked();

        void                        on__pb_i_ready_clicked(bool checked);

        void                        on__pb_begin_clicked();

    private:

        bool                        isReadyToStart();

        void                        createSlots();

        void                        updateSlots();

        void                        hideLastSlots(int cant = 0);

        QList<PlayerData>           _players_data;

        Ui::FormCreatedGame*        _ui;

        TcpGameServer*              _server_game;

        TcpGameConnectionClient*    _client_game;

        QList<Slot>                 _slots;

        QTimer*                     _timer_for_request_latency;

        int                         _index_of_terrain;
        QPixmap                     _presentation;

        int                         _counter_of_player;

        int                         _green_counter;

        int                         _red_counter;
};

#endif // FORMCREATEDGAME_H
