#ifndef TANK_H
#define TANK_H

//#include <QtGui>

#include "itank.h"
#include "skill1.h"
#include "skill2.h"
#include "skill3.h"

#define gt_plugin "xavier_tank_pantanoso_v1";

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

        void                    setCurrentLive(double current_live, ITank* tank_cause_change, bool animate_numbers = true);

        void                    setActivateSkill3(bool value);

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

        bool                    _skill_3_is_activated;
        QList<ITank*>          _tanks_for_skill_3;
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
            _tank_info._name                        = "Pantanoso";
            _tank_info._description                 = "Este tanque salió misteriosamente, así como si nada, de un pantano. Por eso se llama así, que curioso heee?";
            _tank_info._preview                     = QPixmap(":/xavier_tank_pantanoso/sprites/preview.png");

            _tank_info._live                        = 360;
            _tank_info._mana                        = 200;
            _tank_info._armor                       = 1;
            _tank_info._speed                       = 26;

            _tank_info._live_regeneration           = 3;
            _tank_info._mana_regeneration           = 1;

            _tank_info._inc_max_live                = 63;
            _tank_info._inc_max_mana                = 35;
            _tank_info._inc_armor                   = 0.6;
            _tank_info._inc_speed                   = 1.3;
            _tank_info._inc_live_regeneration       = 1.2;
            _tank_info._inc_mana_regeneration       = 0.6;

            _tank_info._skill_info_1._name          = "Bola de lodo";
            _tank_info._skill_info_1._description   = "Lanza una bola de lodo, no es muy dañina pero ralentiza al objetivo por un corto período de tiempo.";
            _tank_info._skill_info_1._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_1._name).arg(_tank_info._skill_info_1._description);
            _tank_info._skill_info_1._preview       = QPixmap(":/xavier_tank_pantanoso/sprites/preview_skill_1.png");
            _tank_info._skill_info_1._definitive    = false;

            _tank_info._skill_info_2._name          = "Hojas verdes";
            _tank_info._skill_info_2._description   = "Pantanoso es capaz de curarse así mismo usando solo hojas de los árboles.";
            _tank_info._skill_info_2._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_2._name).arg(_tank_info._skill_info_2._description);
            _tank_info._skill_info_2._preview       = QPixmap(":/xavier_tank_pantanoso/sprites/preview_skill_2.png");
            _tank_info._skill_info_2._definitive    = false;

            _tank_info._skill_info_3._name          = "Púas";
            _tank_info._skill_info_3._description   = "Crea un poderoso escudo de puas venenosas, ocacionandole el doble de daño recibido al enemigo que lo ataque.";
            _tank_info._skill_info_3._details       = QString("<span class = \"b\">%1</span><br/><br/><span>%2</span>").arg(_tank_info._skill_info_3._name).arg(_tank_info._skill_info_3._description);
            _tank_info._skill_info_3._preview       = QPixmap(":/xavier_tank_pantanoso/sprites/preview_skill_3.png");
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
