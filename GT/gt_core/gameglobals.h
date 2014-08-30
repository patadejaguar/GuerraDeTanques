#ifndef GAMEGLOBALS_H
#define GAMEGLOBALS_H

#include <QtGlobal>
#include <QDir>
#include <QSize>
#include <QColor>
#include <QPixmap>

//BEGIN MATH ------------------------------------------------------

    const double    PI                                      = 3.14159265358979323846264338327950288419717;
    const double    PIx2                                    = PI*2;

//END MATH --------------------------------------------------------

//BEGIN GAME CONFIGURATIONS ---------------------------------------

    const int       GAME_VERSION                            = 1;
    const int       MAX_PLAYERS                             = 6;
    const QDir      DIR_TANKS                               = QDir(QDir::currentPath()+"/tanks");
    const QDir      DIR_TERRAINS                            = QDir(QDir::currentPath()+"/terrains");
    const QDir      DIR_OBJECTS                             = QDir(QDir::currentPath()+"/objects");
    const QDir      DIR_REPLAYS                             = QDir(QDir::currentPath()+"/replays");
    const int       MAX_RANK                                = 15;

//END GAME CONFIGURATIONS -----------------------------------------

//BEGIN GRAPHICS UPDATE -------------------------------------------

    const int       TICK                                    = 35;
    const double    FPS                                     = 1000/TICK;
    const int       ONE_SECOND                              = 1000;
    const int       DELAY_BETWEEN_SKILLS                    = 100;

//END GRAPHICS UPDATE ---------------------------------------------

//BEGIN GRAPHICS DETAILS ------------------------------------------

    //resolution --------------------------------------------------
    const QSize     SCREEN_RESOLUTION                       = QSize(800,600);
    //-------------------------------------------------------------

    //colors ------------------------------------------------------
    const QString   COLOR_BLUE                              = QColor(0,162,232).name(QColor::HexArgb);
    const QString   COLOR_BLUE_LIGHT                        = QColor(34,189,255).name(QColor::HexArgb);
    const QString   COLOR_GREEN                             = QColor(0,235,0).name(QColor::HexArgb);
    const QString   COLOR_GREEN_LEMON                       = QColor(180,230,30).name(QColor::HexArgb);
    const QString   COLOR_GREEN_LEMON_LIGHT                 = QColor(123, 226, 42).name(QColor::HexArgb);
    const QString   COLOR_ORANGE                            = QColor(255,128,0).name(QColor::HexArgb);
    const QString   COLOR_ORANGE_DARK                       = QColor(255,81,0).name(QColor::HexArgb);
    const QString   COLOR_RED                               = QColor(255,0,0).name(QColor::HexArgb);
    const QString   COLOR_RED_LIGHT                         = QColor(255,70,70).name(QColor::HexArgb);
    const QString   COLOR_RED_DARK                          = QColor(150,0,0).name(QColor::HexArgb);
    const QString   COLOR_YELOW                             = QColor(255,255,0).name(QColor::HexArgb);
    const QString   COLOR_YELOW_LIGHT                       = QColor(239,228,176).name(QColor::HexArgb);
    const QString   COLOR_PURPLE                            = QColor(163,73,164).name(QColor::HexArgb);
    const QString   COLOR_WHITE                             = QColor(255,255,255).name(QColor::HexArgb);
    //-------------------------------------------------------------

//END GRAPHICS DETAILS --------------------------------------------

//BEGIN MECHANICS -------------------------------------------------

    const int       MECH_MAX_TANK_LEVEL                     = 12;
    const int       MECH_MIN_TANK_SPEED                     = 0;
    const int       MECH_MAX_TANK_SPEED                     = 60;
    const int       MECH_MAX_DISTANCE_SOUND                 = 1000;
    const int       MECH_DEAD_TIME_FOR_LEVEL                = 3000;
    const int       MECH_INIT_NEXT_LEVEL_EXPERIENCE         = 200;

//END MECHANICS ---------------------------------------------------

//BEGIN CLIENT-SERVER(UI) -----------------------------------------

    //user data ---------------------------------------------------
    struct UserProperties{

            QString                 _nick;

            QPixmap                 _avatar;

            int                     _wins;

            int                     _losses;

            void                    clear(){
                                    _nick.clear();
                                    _avatar = QPixmap();
                                    _wins = _losses = -1;
            }

            int                     rank() {
                                    int rank = _wins - _losses;
                                    if(rank >= 40)
                                        return 15;
                                    else if(rank >= 30)
                                        return 14;
                                    else if(rank >= 24)
                                        return 13;
                                    else if(rank >= 22)
                                        return 12;
                                    else if(rank >= 20)
                                        return 11;
                                    else if(rank >= 18)
                                        return 10;
                                    else if(rank >= 16)
                                        return 9;
                                    else if(rank >= 14)
                                        return 8;
                                    else if(rank >= 12)
                                        return 7;
                                    else if(rank >= 10)
                                        return 6;
                                    else if(rank >= 8)
                                        return 5;
                                    else if(rank >= 6)
                                        return 4;
                                    else if(rank >= 4)
                                        return 3;
                                    else if(rank >= 2)
                                        return 2;                                    
                                    else
                                        return 1;
            }

    };
    Q_DECLARE_METATYPE(UserProperties)
    inline QDataStream &operator<<(QDataStream &stream, const UserProperties &up)
    {
        stream << up._nick << up._avatar << up._wins << up._losses;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, UserProperties &up)
    {
        stream >> up._nick >> up._avatar >> up._wins >> up._losses;
        return stream;
    }
    inline bool operator==(UserProperties my, const  UserProperties &up)
    {
        return my._nick == up._nick;
    }
    //-------------------------------------------------------------

    //user commands -----------------------------------------------
    const quint8    TCP_CONNECTED_DATA                      = 1;
    const quint8    TCP_MISSING_RESOURCES                   = 2;
    const quint8    TCP_REGISTER_USER                       = 3;
    const quint8    TCP_REGISTER_USER_SUCCESSFUL            = 4;
    const quint8    TCP_REGISTER_USER_ERROR                 = 5;

    const quint8    TCP_EDIT_USER                           = 6;
    const quint8    TCP_EDIT_USER_SUCCESSFUL                = 7;
    const quint8    TCP_EDIT_USER_ERROR                     = 8;

    const quint8    TCP_LOGIN_USER                          = 9;
    const quint8    TCP_LOGIN_USER_SUCCESSFUL               = 10;
    const quint8    TCP_LOGIN_USER_ERROR                    = 11;

    const quint8    TCP_LOGOUT_USER                         = 12;
    //-------------------------------------------------------------

    //message commands --------------------------------------------
    const quint8    TCP_MESSAGE                             = 21;
    //-------------------------------------------------------------

    //game commands -----------------------------------------------
    const quint8    TCP_CREATE_GAME                         = 41;
    const quint8    TCP_CLOSE_GAME                          = 42;
    const quint8    TCP_USE_GAME                            = 43;
    const quint8    TCP_REQUEST_CREATED_GAMES               = 44;
    const quint8    TCP_PLAYER_JOINED_GAME                  = 45;
    const quint8    TCP_PLAYER_LEAVED_GAME                  = 46;
    const quint8    TCP_GAME_STARTED                        = 47;
    const quint8    TCP_GAME_OVER                           = 48;
    //-------------------------------------------------------------

    //CONNECTION --------------------------------------------------
    const QString   TCP_LOCATION_IN_CHAT                    = "in_chat";
    const QString   TCP_LOCATION_IN_GAME                    = "in_game_of_%1";
    //-------------------------------------------------------------

//END CLIENT-SERVER(UI) -------------------------------------------

//BEGIN SERVER(GAME) ----------------------------------------------

    //structs -----------------------------------------------------
    struct PlayerData{
        QString             _nick;
        QPixmap             _avatar;
        int                 _rank;
        int                 _team;
        int                 _index_of_tank;
        bool                _ready;

        void                changeTeam(){_team = (_team == 1) ? 2 : 1;}
        void                changeTank(int index_of_tank){_index_of_tank = index_of_tank;}
        void                setReady(bool value){_ready = value;}
    };
    Q_DECLARE_METATYPE(PlayerData)
    Q_DECLARE_METATYPE(QList<PlayerData>)

    inline QDataStream &operator<<(QDataStream &stream, const PlayerData &pd)
    {
        stream << pd._nick << pd._avatar << pd._rank << pd._team << pd._index_of_tank << pd._ready;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, PlayerData &pd)
    {
        stream >> pd._nick >> pd._avatar >> pd._rank >> pd._team >> pd._index_of_tank >> pd._ready;
        return stream;
    }
    inline bool operator==(PlayerData my, const  PlayerData &pd)
    {
        return my._nick == pd._nick;
    }
    inline bool operator<(PlayerData my, const  PlayerData &pd)
    {
        return my._nick < pd._nick;
    }

    struct PlayerAction{
            qint8           _player_index;

            QString         _message;
            bool            _for_all;

            bool            _time_control_change;
            bool            _pause_game;

            double          _turret_rotation;

            bool            _data_control_changed;
            bool            _move_fordward, _move_backward, _rotate_left, _rotate_right;
            bool            _execute_skill_1, _execute_skill_2, _execute_skill_3;
            bool            _update_skill_1, _update_skill_2, _update_skill_3;

    };
    Q_DECLARE_METATYPE(PlayerAction)

    inline QDataStream &operator<<(QDataStream &stream, const PlayerAction &pa)
    {
        stream << pa._player_index;

        stream << pa._message << pa._for_all;

        stream << pa._turret_rotation;

        stream << pa._time_control_change << pa._pause_game;

        stream << pa._data_control_changed;
        stream << pa._move_fordward << pa._move_backward << pa._rotate_left << pa._rotate_right;
        stream << pa._execute_skill_1 << pa._execute_skill_2 << pa._execute_skill_3;
        stream << pa._update_skill_1 << pa._update_skill_2 << pa._update_skill_3;

        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, PlayerAction &pa)
    {
        stream >> pa._player_index;

        stream >> pa._message >> pa._for_all;

        stream >> pa._turret_rotation;

        stream >> pa._time_control_change >> pa._pause_game;

        stream >> pa._data_control_changed;
        stream >> pa._move_fordward >> pa._move_backward >> pa._rotate_left >> pa._rotate_right;
        stream >> pa._execute_skill_1 >> pa._execute_skill_2 >> pa._execute_skill_3;
        stream >> pa._update_skill_1 >> pa._update_skill_2 >> pa._update_skill_3;

        return stream;
    }

    struct Actions{
        QList<PlayerAction> _player_actions;
    };
    Q_DECLARE_METATYPE(Actions)

    inline QDataStream &operator<<(QDataStream &stream, const Actions &actions)
    {
        stream << actions._player_actions;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, Actions &actions)
    {
        stream >> actions._player_actions;
        return stream;
    }

    struct KillsDeaths{
            int         _kills;
            int         _deaths;
            bool        _circle_of_power_destroyed;
    };
    inline QDataStream &operator<<(QDataStream &stream, const KillsDeaths &kills_deaths)
    {
        stream << kills_deaths._kills << kills_deaths._deaths << kills_deaths._circle_of_power_destroyed;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, KillsDeaths &kills_deaths)
    {
        stream >> kills_deaths._kills >> kills_deaths._deaths >> kills_deaths._circle_of_power_destroyed;
        return stream;
    }

    struct PlayerInformation{
        QString             _nick;
        QPixmap             _avatar;
        int                 _rank;
        int                 _team;
        QString             _tank_name;
    };
    Q_DECLARE_METATYPE(PlayerInformation)
    Q_DECLARE_METATYPE(QList<PlayerInformation>)

    inline QDataStream &operator<<(QDataStream &stream, const PlayerInformation &pi)
    {
        stream << pi._nick << pi._avatar << pi._rank << pi._team << pi._tank_name;
        return stream;
    }
    inline QDataStream &operator>>(QDataStream &stream, PlayerInformation &pi)
    {
        stream >> pi._nick >> pi._avatar >> pi._rank >> pi._team >> pi._tank_name;
        return stream;
    }
    //-------------------------------------------------------------

    //general -----------------------------------------------------
    const qint16    TCP_GAME_SERVER_PORT                    = 9091;
    //-------------------------------------------------------------

    const quint8    TCP_WORLD_UPDATE                        = 100;
    const quint8    TCP_DELAY_COMMAND                       = TICK*2;

    const int       TCP_REQUEST_LATENCY_TIME                = 2000;
    //commands ----------------------------------------------------
    const quint8    TCP_COMMAND_REQUEST_DATA                = 1;
    const quint8    TCP_COMMAND_DATA_RESPONCE               = 2;

    const quint8    TCP_COMMAND_PLAYER_JOINED_SUCCESSFUL    = 3;
    const quint8    TCP_COMMAND_NEW_PLAYER_JOINED           = 4;
    const quint8    TCP_COMMAND_PLAYER_LEAVE_GAME           = 5;

    const quint8    TCP_COMMAND_MESSAGE                     = 6;

    const quint8    TCP_COMMAND_CHANGE_TEAM                 = 7;
    const quint8    TCP_COMMAND_CHANGE_TANK                 = 8;

    const quint8    TCP_COMMAND_PING                        = 9;
    const quint8    TCP_COMMAND_PONG                        = 10;
    const quint8    TCP_COMMAND_LATENCY                     = 11;

    const quint8    TCP_COMMAND_PLAYER_READY                = 12;

    const quint8    TCP_COMMAND_PREPARE_GAME                = 13;
    const quint8    TCP_COMMAND_GAME_READY                  = 14;

    const quint8    TCP_COMMAND_GAME_FULL                   = 63;
    const quint8    TCP_COMMAND_GAME_READY_TO_START         = 64;
    const quint8    TCP_COMMAND_GAME_IS_STARTED             = 65;
    const quint8    TCP_COMMAND_START_SYNC                  = 66;
    const quint8    TCP_COMMAND_END_SYNC                    = 67;
    const quint8    TCP_COMMAND_START_GAME_IN_5_SEC         = 68;
    const quint8    TCP_COMMAND_PLAYER_ACTION               = 69;
    const quint8    TCP_COMMAND_WORD_CHANGE                 = 70;

    const quint8    TCP_COMMAND_PAUSE_GAME                  = 71;
    const quint8    TCP_COMMAND_RESUME_GAME                 = 72;

    const quint8    TCP_COMMAND_GAME_MESSAGE                = 73;
    //-------------------------------------------------------------
//END SERVER(GAME) ------------------------------------------------

#endif // GAMEGLOBALS_H
