#ifndef TANK_H
#define TANK_H

//#include <QtGui>

#include "itank.h"
#include "skill1.h"
#include "skill2.h"
#include "skill3.h"

#define gt_plugin "xavier_tank_arenas_v1";

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
            _tank_info._name                        = "Arenas";
            _tank_info._description                 = "Este tanque lo hizo el tataratataratataranieto del mismísimo Tutancamon con bloques y arena de las pirámides de Egipto.";
            _tank_info._preview                     = QPixmap(":/xavier_tank_arenas/sprites/preview.png");

            _tank_info._live                        = 300;
            _tank_info._mana                        = 400;
            _tank_info._armor                       = 0;
            _tank_info._speed                       = 30;

            _tank_info._live_regeneration           = 2;
            _tank_info._mana_regeneration           = 1;

            _tank_info._inc_max_live                = 80;
            _tank_info._inc_max_mana                = 30;
            _tank_info._inc_armor                   = 0.2;
            _tank_info._inc_speed                   = 0.4;
            _tank_info._inc_live_regeneration       = 0.5;
            _tank_info._inc_mana_regeneration       = 0.5;

            _tank_info._skill_info_1._name          = "Bola de arena";
            _tank_info._skill_info_1._description   = "Concentra su poder en una bola de arena que solo afecta a sus enemigos, ocasionándoles daño.";
            _tank_info._skill_info_1._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_1._name).arg(_tank_info._skill_info_1._description);
            _tank_info._skill_info_1._preview       = QPixmap(":/xavier_tank_arenas/sprites/preview_skill_1.png");
            _tank_info._skill_info_1._definitive    = false;

            _tank_info._skill_info_2._name          = "Escudo de arena";
            _tank_info._skill_info_2._description   = "Aumentanda su armadura por un período de 5 segundos.";
            _tank_info._skill_info_2._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_2._name).arg(_tank_info._skill_info_2._description);
            _tank_info._skill_info_2._preview       = QPixmap(":/xavier_tank_arenas/sprites/preview_skill_2.png");
            _tank_info._skill_info_2._definitive    = false;

            _tank_info._skill_info_3._name          = "Bola de fuego";
            _tank_info._skill_info_3._description   = "Lanza una poderosa bola de fuego que solo afecta a sus enemigos, ocasionándoles daño y disminuyendo su armadura por un período de 5 segundos.";
            _tank_info._skill_info_3._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_3._name).arg(_tank_info._skill_info_3._description);
            _tank_info._skill_info_3._preview       = QPixmap(":/xavier_tank_arenas/sprites/preview_skill_3.png");
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
