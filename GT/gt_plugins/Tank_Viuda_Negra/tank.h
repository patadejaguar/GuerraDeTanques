#ifndef TANK_H
#define TANK_H

//#include <QtGui>

#include "itank.h"
#include "skill1.h"
#include "skill2.h"
#include "skill3.h"

#define gt_plugin "xavier_tank_viuda_negra_v1";

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
            _tank_info._name                        = "Viuda Negra";
            _tank_info._description                 = "Este tanque lo hizo el hombre araña cuando se aburrió de tirar telitas de araña por la ciudad.";
            _tank_info._preview                     = QPixmap(":/xavier_tank_viuda_negra/sprites/preview.png");

            _tank_info._live                        = 280;
            _tank_info._mana                        = 300;
            _tank_info._armor                       = 3;
            _tank_info._speed                       = 32;

            _tank_info._live_regeneration           = 1;
            _tank_info._mana_regeneration           = 1;

            _tank_info._inc_max_live                = 75;
            _tank_info._inc_max_mana                = 34;
            _tank_info._inc_armor                   = 0.5;
            _tank_info._inc_speed                   = 0.3;
            _tank_info._inc_live_regeneration       = 0.6;
            _tank_info._inc_mana_regeneration       = 0.4;

            _tank_info._skill_info_1._name          = "Bola pegajosa";
            _tank_info._skill_info_1._description   = "Lanza una bola comprimida de su tela pegajosa, ralentizando al enemigo que alcance por 4 segundos.";
            _tank_info._skill_info_1._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_1._name).arg(_tank_info._skill_info_1._description);
            _tank_info._skill_info_1._preview       = QPixmap(":/xavier_tank_viuda_negra/sprites/preview_skill_1.png");
            _tank_info._skill_info_1._definitive    = false;

            _tank_info._skill_info_2._name          = "Trampa de araña";
            _tank_info._skill_info_2._description   = "Pone una trampa en el suelo. Cuando un enemigo la toca se activa, disminuyendo su velocidad de movimiento a cero por un período de tiempo. Dura 2 minutos.";
            _tank_info._skill_info_2._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_2._name).arg(_tank_info._skill_info_2._description);
            _tank_info._skill_info_2._preview       = QPixmap(":/xavier_tank_viuda_negra/sprites/preview_skill_2.png");
            _tank_info._skill_info_2._definitive    = false;

            _tank_info._skill_info_3._name          = "Bola pegajosa envenenada";
            _tank_info._skill_info_3._description   = "Lanza una bola comprimida de su tela envenenada, ocacionando que su enemigo pierda 60 vida/seg por un período de tiempo.";
            _tank_info._skill_info_3._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_3._name).arg(_tank_info._skill_info_3._description);
            _tank_info._skill_info_3._preview       = QPixmap(":/xavier_tank_viuda_negra/sprites/preview_skill_3.png");
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
