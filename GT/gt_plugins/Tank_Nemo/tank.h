#ifndef TANK_H
#define TANK_H

//#include <QtGui>

#include "itank.h"
#include "skill1.h"
#include "skill2.h"
#include "skill3.h"

#define gt_plugin "xavier_tank_nemo_v1";

class Tank: public ITank
{
        Q_OBJECT

    public:

        explicit                Tank(TankInfo tank_info, QString nick, int rank, ColorTeam color_team);

        ~Tank();

        //funciones utilizada para la detección de colisiones
        QPainterPath            shape() const;

        //instancia las cuatro habilidades
        void                    createSkills();

        //posición de la torreta
        QPoint                  getTurretPosition();

    protected:

        //funciones para la visualización de enemigos y amigos
        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        //función para el ciclo de eventos del tanque
        void                    gameLoop();

        //funciones para mover y rotar el tanque
        void                    move();
        void                    rotate();

        void                    reborn();

    private:

        QList<QPixmap>          _animation_whell;
        int                     _frame_animation_whell_1;
        int                     _frame_animation_whell_2;

        QPixmap                 _body;
        QPixmap                 _turret;

        QRectF                  _rect_body;
        QPainterPath            _shape_body;

        QPoint                  _body_position;
        QPoint                  _turret_position;
        QPoint                  _whell_1_position;
        QPoint                  _whell_2_position;
};

class TankFactory: public QObject, public ITankFactory
{
        Q_OBJECT
        Q_INTERFACES(ITankFactory)
        Q_PLUGIN_METADATA(IID "xavier.gt.itankfactory")

    public:

        inline QString          pluginName()        {return gt_plugin;}
        inline int              gameVersion()       {return 1;}

        inline                  TankFactory()
        {
            _tank_info._name                        = "Nemo";
            _tank_info._description                 = "Este tanque lo hizo el capitán Nemo después de aburrise de navegar en el Nautilus.";
            _tank_info._preview                     = QPixmap(":/xavier_tank_nemo/sprites/preview.png");

            _tank_info._live                        = 240;
            _tank_info._mana                        = 400;
            _tank_info._armor                       = 1;
            _tank_info._speed                       = 27;

            _tank_info._live_regeneration           = 1;
            _tank_info._mana_regeneration           = 3;

            _tank_info._inc_max_live                = 55;
            _tank_info._inc_max_mana                = 60;
            _tank_info._inc_armor                   = 0.8;
            _tank_info._inc_speed                   = 1.2;
            _tank_info._inc_live_regeneration       = 0.8;
            _tank_info._inc_mana_regeneration       = 1.3;

            _tank_info._skill_info_1._name          = "Bola de coral";
            _tank_info._skill_info_1._description   = "Lanza un poderoso proyectil hecho de coral que solo afecta a sus enemigos.";
            _tank_info._skill_info_1._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_1._name).arg(_tank_info._skill_info_1._description);
            _tank_info._skill_info_1._preview       = QPixmap(":/xavier_tank_nemo/sprites/preview_skill_1.png");
            _tank_info._skill_info_1._definitive    = false;

            _tank_info._skill_info_2._name          = "Gota de elixir";
            _tank_info._skill_info_2._description   = "Una vez activado regenera mana a él y sus aliados en un área de 500u.";
            _tank_info._skill_info_2._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_2._name).arg(_tank_info._skill_info_2._description);
            _tank_info._skill_info_2._preview       = QPixmap(":/xavier_tank_nemo/sprites/preview_skill_2.png");
            _tank_info._skill_info_2._definitive    = false;

            _tank_info._skill_info_3._name          = "El guardián de las perlas";
            _tank_info._skill_info_3._description   = "Dota a Nemo y sus aleados cercanos en un área de 300u de un poderoso escudo, protegiendolos del daño por un período de tiempo.";
            _tank_info._skill_info_3._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_3._name).arg(_tank_info._skill_info_3._description);
            _tank_info._skill_info_3._preview       = QPixmap(":/xavier_tank_nemo/sprites/preview_skill_3.png");
            _tank_info._skill_info_3._definitive    = true;
        }        

        inline ITank*           createInstance(QString nick, int rank, ColorTeam color_team)
        {
            ITank* tank = new Tank(_tank_info, nick, rank, color_team);
            tank->createSkills();
            return tank;
        }
};

#endif // TANK_H
