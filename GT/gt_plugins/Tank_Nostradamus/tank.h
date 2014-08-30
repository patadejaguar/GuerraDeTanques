#ifndef TANK_H
#define TANK_H

//#include <QtGui>

#include "itank.h"
#include "skill1.h"
#include "skill2.h"
#include "skill3.h"

#define gt_plugin "xavier_tank_nostradamus_v1";

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
            _tank_info._name                        = "Nostradamus";
            _tank_info._description                 = "Este tanque lo hizo Nostradamus, así como por arte de magia.";
            _tank_info._preview                     = QPixmap(":/xavier_tank_nostradamus/sprites/preview.png");

            _tank_info._live                        = 200;
            _tank_info._mana                        = 450;
            _tank_info._armor                       = 2;
            _tank_info._speed                       = 32;

            _tank_info._live_regeneration           = 1;
            _tank_info._mana_regeneration           = 2;

            _tank_info._inc_max_live                = 75;
            _tank_info._inc_max_mana                = 50;
            _tank_info._inc_armor                   = 0.3;
            _tank_info._inc_speed                   = 0.3;
            _tank_info._inc_live_regeneration       = 0.6;
            _tank_info._inc_mana_regeneration       = 0.8;

            _tank_info._skill_info_1._name          = "Energía pura";
            _tank_info._skill_info_1._description   = "Lanza un proyectil de pura energía que solo afecta a sus enemigos, ocasionándoles daño.";
            _tank_info._skill_info_1._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_1._name).arg(_tank_info._skill_info_1._description);
            _tank_info._skill_info_1._preview       = QPixmap(":/xavier_tank_nostradamus/sprites/preview_skill_1.png");
            _tank_info._skill_info_1._definitive    = false;

            _tank_info._skill_info_2._name          = "Ladrón de mana";
            _tank_info._skill_info_2._description   = "Regenera mana robada de los enemigos cercanos en un área de 400u.";
            _tank_info._skill_info_2._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_2._name).arg(_tank_info._skill_info_2._description);
            _tank_info._skill_info_2._preview       = QPixmap(":/xavier_tank_nostradamus/sprites/preview_skill_2.png");
            _tank_info._skill_info_2._definitive    = false;

            _tank_info._skill_info_3._name          = "Energía negativa";
            _tank_info._skill_info_3._description   = "Lanza una poderosa bola de energía ocacionando daño a los enemigos basados en el mana gastado.";
            _tank_info._skill_info_3._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_3._name).arg(_tank_info._skill_info_3._description);
            _tank_info._skill_info_3._preview       = QPixmap(":/xavier_tank_nostradamus/sprites/preview_skill_3.png");
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
