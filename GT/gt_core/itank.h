#ifndef ITANK_H
#define ITANK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>

#include "gameglobals.h"
#include "effect.h"
#include "iskill.h"
#include "infobar.h"
#include "utilities.h"

class SoundEngine;
class CircleOfPower;
class ITankFactory;

struct GT_CORESHARED_EXPORT TankInfo
{
        QString     _name;
        QString     _description;
        QPixmap     _preview;

        double      _live;
        double      _mana;
        double      _armor;
        double      _speed;
        double      _live_regeneration;
        double      _mana_regeneration;
        double      _inc_max_live;
        double      _inc_live_regeneration;
        double      _inc_max_mana;
        double      _inc_mana_regeneration;
        double      _inc_armor;
        double      _inc_speed;

        SkillInfo   _skill_info_1;
        SkillInfo   _skill_info_2;
        SkillInfo   _skill_info_3;
};

/**
 *La clase abstracta ITank es usada para definir las funcionalidades de los tanques en el juego.
 *
 *@author Ing. Javier Bandomo Ruíz
 */

class GT_CORESHARED_EXPORT ITank : public QGraphicsObject
{
        Q_OBJECT

        friend class CircleOfPower;

    public:

        /**
         * Inicializa todos los atributos del tanque.
         */
        explicit                ITank(TankInfo tank_info, QString nick, int rank, ColorTeam color_team);

        /**
         * Destructor de la clase tanque.
          */
        virtual                 ~ITank();

        TankInfo                tankInfo() const;

        /**
         * Referencia al jugador principal (por cliente).
         */
        ITank*                  principalReference();

        void                    setPrincipalReference(ITank* tank);

        /**
         * Cambia las limitaciones de región del tanque.
         */
        void                    setCorner(int corner_vertical, int corner_horizontal);

        /**
         * Crea la barra de información
         */
        void                    createInfoBar();

        /**
         * Retorna el color del equipo.
         */
        ColorTeam               getColorTeam();

        /**
          * Retorna el nombre del tanque.
          */
        QString                 getName();

        /**
         * Retorna la descripción del tanque.
         */
        QString                 getDescription();

        /**
         * Retorna la vista previa del tanque. 55 x 55
         */
        QPixmap                 getPreview();

        QGraphicsPixmapItem*    miniMapTank() const;

        /**
         * Retorna el valor del atributo _is_enemy.
         */
        bool                    isEnemy();

        /**
         * Cambia el valor del atributo @b _is_enemy por @b value.
         */
        void                    setIsEnemy(bool value);

        /**
         * El objetivo de este método es crear las instancias de las habilidades del tanque.
         * @see ISkill
         */
        virtual void            createSkills() = 0;

        /**
         * Retorna la habilidad en el índice @b index.
         * @see isHurledSkill(inr index)
         * @see setHurledSkill(int index, bool value)
        */
        ISkillFactory*          getSkill(int index);

        /**
         * Retorna verdadero si la habilidad en la posición @b index está lanzada, de lo contrario retorna false.
         * @see getSkill(int index)
         * @see setHurledSkill(int index, bool value)
         */
        bool                    isHurledSkill(int index);

        /**
         * Cambia el estado de la habilidad en en índice @b index por el valor @b value.
         * @see isHurledSkill(int index)
         * @see getSkill(int index)
         */
        void                    setHurledSkill(int index, bool value);

        /**
         * Cambia las coordenadas del tanque a la posición @b pos.
         * @see setPos(qreal x, qreal y)
         * @see notifyPosition(QPointF)
         */
        virtual void            setPos(QPointF pos);
        /**
         * Cambia la posición del tanque al valor de @b x y @b y.
         * @see setPos(QPointF pos)
         */
        virtual void            setPos(qreal x, qreal y);

        /**
         * Retorna el valor del atributo @b _angle
         * @see setRotation(double angle)
         * @see getRotationTurret()
         * @see setRotationTurret(qreal turret_angle)
         */
        double                  getRotation();

        /**
         * Cambia el valor de _angle por @b angle.
         * @see getRotation()
         * @see getRotationTurret()
         * @see setRotationTurret(qreal turret_angle)
         * @see notifyRotation(int)
         */
        virtual void            setRotation(double angle);

        /**
         * Retorna el valor del atributo @b _turret_angle.
         * @see setRotationTurret(qreal turret_angle)
         * @see getRotation()
         * @see setRotation(double angle)
         */
        double                  turretRotation();

        /**
         * Cambia el valor del atributo @b _turret_angle por @b turret_angle.
         * @see getRotationTurret()
         * @see getRotation()
         * @see setRotation(double angle)
         * @see notifyRotationTurret(int)
         */
        virtual void            setTurretRotation(qreal turret_angle);

        /**
         * Devuelve el valor del atributo _current_live.
         * @see applyDamage(int value, ITank *tank)
         * @see getMaxLive()
         * @see setMaxLive(int max_live)
         */
        int                     currentLive();

        /**
         * Cambia el valor del atributo @b _current_live por @b current_live
         * @see getCurrentLive()
         * @see getMaxLive()
         * @see setMaxLive(int max_live)
         * @see notifyCurrentLive(int)
         */
        virtual void            setCurrentLive(double current_live, ITank* tank_cause_change, bool animate_numbers = true);

        /**
         * Retorna el valor del atributo @b _max_live.
         * @see setMaxLive(int max_live)
         * @see getCurrentLive()
         * @see applyDamage(int value, ITank *tank)
         */
        int                     maxLive();

        /**
         * Cambia el valor del atributo @b _max_live por @b max_live
         * @see getMaxLive()
         * @see getCurrentLive()
         * @see applyDamage(int value, ITank *tank)
         * @see notifyMaxLive(int)
         */
        virtual void            setMaxLive(double max_live);

        /**
         * Retorna el valor del atributo @b _current_mana.
         * @see setCurrentMana(int current_mana)
         * @see getMaxMana()
         * @see setMaxMana(int max_mana)
         */
        int                     currentMana();

        /**
         * Cambia el valor del atributo @b _current_mana por @b current_mana
         * @see getCurrentMana()
         * @see getMaxMana()
         * @see setMaxMana(int max_mana)
         * @see notifyCurrentMana(int)
         */
        virtual void            setCurrentMana(double current_mana, bool animate_numbers = true);

        /**
         * Retorna el valor del atributo @b _max_mana.
         * @see setMaxMana(int max_mana);
         * @see getCurrentMana()
         * @see setCurrentMana(int max_mana)
         */
        int                     maxMana();

        /**
         * Cambia el valor del atributo @b _max_mana por @b max_mana
         * @see getMaxMana()
         * @see getCurrentMana()
         * @see setCurrentMana(int max_mana)
         * @see notifyMaxMana(int)
         */
        virtual void            setMaxMana(double max_mana);

        /**
         * Retorna el valor del atributo @b _base_live_regeneration.
         * @see getBonusLiveRegeneration()
         */
        int                     getBaseLiveRegeneration();

        /**
         * Retorna el valor del atributo @b _bonus_live_regeneration.
         * @see getBaseLiveRegeneration()
         */
        int                     getBonusLiveRegeneration();

        /**
         * Retorna el valor total de la regeneración de vida @_base_live_regeneration +  @b _bonus_live_regeneration.
         * @see getBonusLiveRegeneration()
         * @see getBaseLiveRegeneration()
         */
        int                     getTotalLiveRegeneration();

        /**
         * Retorna el valor del atributo @b _base_mana_regeneration.
         * @see getBonusManaRegeneration()
         */
        int                     getBaseManaRegeneration();

        /**
         * Retorna el valor del atributo @b _bonus_mana_regeneration.
         * @see setBonusManaRegeneration(int bonus_mana_regeneration)
         * @see getBaseManaRegeneration()
         */
        int                     getBonusManaRegeneration();

        /**
         * Retorna el valor total de la regeneración de energía @_base_mana_regeneration +  @b _bonus_mana_regeneration.
         * @see getBonusManaRegeneration()
         * @see getBaseManaRegeneration()
         */
        int                     getTotalManaRegeneration();

        /**
         * Retorna el valor del atributo @b _base_armor.
         * @see getBonusArmor()
         */
        int                     baseArmor();

        /**
         * Retorna el valor del atributo @b _bonus_armor.
         * @see getBaseArmor()
         */
        int                     getBonusArmor();

        /**
         * Retorna el valor total de la regeneración de armadura @_base_armor +  @b _bonus_armor.
         * @see getBonusArmor()
         * @see getBaseArmor()
         */
        int                     getTotalArmor();

        /**
         * Retorna el valor del atributo @b _base_speed.
         * @see getBonusSpeed()
         * @see setBonusSpeed(int bonus_speed)
         */
        double                  baseSpeed();

        /**
         * Retorna el valor del atributo @b _bonus_speed.
         * @see setBonusSpeed(int bonus_speed)
         * @see getBaseSpeed()
         */
        double                  getBonusSpeed();

        /**
         * Retorna el valor total de la regeneración de velocidad @b _base_speed +  @b _bonus_speed.
         * El total de devuelve en un rango entre [0 y 600].
         * @see getBonusSpeed()
         * @see getBaseSpeed()
         */
        double                  getTotalSpeed();

        /**
         * Retorna el valor del atributo @b _status.
         * @see Tank_Status
         */
        Tank_Status             getStatus();

        /**
         * Cambia el valor del atributo @b _status por @b value.
         * @see Tank_Status
         * @see notifyStatus(Tank_Status)
         */
        virtual void            setStatus(Tank_Status value);

        /**
         * Retorna el valor del atributo @b _current_experience.
         * @see getNextLevelExperience()
         * @see addExperience(int)
         */
        int                     getCurrentExperience();

        /**
         * Retorna el valor del atributo @b _next_level_experience.
         * @see getCurrentExperience()
         * @see addExperience(int)
         */
        int                     getNextLevelExperience();

        /**
         * Adiciona al atributo @b _current_experience el valor de @b value.
         * Si la experiencia alcanza el valor del atributo @b _next_level_experience sube de nivel.
         * @see levelUp();
         * @see notifyExperience(int, int)
         * @see getCurrentExperience()
         * @see getNextLevelExperience()
         */
        void                    addExperience(int value);

        /**
         * Retorna el valor del atributo @b _level.
         * @see levelUp()
         */
        int                     getLevel();

        /**
         * Retorna el valor del atributo @b _updates_available.
         */
        int                     getUpdatesAvailablesForSkills();

        /**
         * Sube el nivel de la habilidad número @b index.
         * @see ISkill::updateAttributes()
         */
        void                    updateSkill(int index);

        /**
         * Activa la habilidad.
         */
        void                    activeSkill(int index);

        /**
         * Retorna el valor del atributo @b _(dead_time.
         * @see setDeadTime(int dead_time)
         */
        int                     getDeadTime();

        /**
         * Cambia el valor del atributo @b _dead_time por @b dead_time.
         * @see getDeadTime()
         */
        void                    setDeadTime(int dead_time);

        /**
         * Retorna verdadero si el tanque se está moviendo adelante.
         * @see setMoveForward(bool value)
         */
        bool                    isMoveForward();

        /**
         * Cambia el valor del atributo @b _move_forward por @b value.
         * @see isMoveForward()
         */
        void                    setMoveForward(bool value);

        /**
         * Retorna verdadero si el tanque se está moviendo atrás.
         * @see setMoveBackward(bool value)
         */
        bool                    isMoveBackward();

        /**
         * Cambia el valor del atributo @b _move_backward por @b value.
         * @see isMoveBackward()
         */
        void                    setMoveBackward(bool value);

        /**
         * Retorna verdadero si el tanque está girando hacia la izquierda.
         * @see setRotateLeft(bool value)
         */
        bool                    isRotateLeft();

        /**
         * Cambia el valor del atributo @b _rotate_left por @b value.
         * @see isRotateLeft()
         */
        void                    setRotateLeft(bool value);

        /**
         * Retorna verdadero si el tanque está rotando hacia la derecha.
         * @see setRotateRight(bool value)
         */
        bool                    isRotateRight();

        /**
         * Cambia el valor del atributo @b _rotate_right por @b value.
         * @see isRotateRight()
         */
        void                    setRotateRight(bool value);

        /**
         * Retorna el listado de los tanques aliados.
         * @see addAllied(Itank *tank)
         */
        QList<ITank *>          getAllies();

        /**
         * Retorna el listado de los tanques enemigos.
         ** @see addEnemy(Itank *tank)
         */
        QList<ITank *>          getEnemie();

        /**
         * Retorna el listado de todos los tanques, menos este.
         ** @see addOtherTank(Itank *tank)
         */
        QList<ITank *>          getOthersTanks();

        /**
         * Adiciona el tanque @b tank a la lista de aliados.
         * @see getAllies()
         */
        void                    addAllied(ITank* tank);

        /**
         * Adiciona el tanque @b tank a la lista de enemigos.
         * @see getEnemie()
         */
        void                    addEnemy(ITank* tank);

        /**
         * Cambia el valor del atributo @b _obstructed_areas por @b areas.
         * @see collidesWithObstructedAreas()
         */
        void                    setObstructecAreas(QList<QPainterPath> areas);

        virtual QRectF          boundingRect() const;

        //funciones para visualización en clientes enemigos y amigos
        virtual void            paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        /**
         * Adiciona modificaciones a los atributos del tanque (@b _base_live_regeneration,
         * @b _base_mana_regeneration, @b _base_armor, @b _base_speed)
         * @see removeBonus(Bonus bonus)
         * @see notifyEffect()
         */
        void                    addEffect(Effect* effect);

        /**
         * Elimina una modificación añadida previamente.
         * @see addBonus(Bonus bonus)
         * @see notifyEffect()
         */
        void                    removeEffect(Effect* effect);

        void                    removeAllEffects();

        /**
         * Retorna el valor de las areas obstruidas
         */
        QList<QPainterPath>     getObstructedAreas();

        /**
         * Esta función solo llama al método reborn.
         */
        void                    initTank();

        CircleOfPower*          circle_of_power() const;
        void                    setCircleOfPower(CircleOfPower *circle_of_power);

        void                    addOtherCircleOfPower(CircleOfPower *circle_of_power);

    public slots:

        void                    advance(int phase);

    signals:

        /**
         * Notifica del cambio de la posición del tanque
         */
        void                    notifyPosition(QPointF);

        /**
         * Notifica del cambio de la rotación del tanque.
         */
        void                    notifyRotation(int);

        /**
         * Notifica del cambio rotación de la torreta del tanque.
         */
        void                    notifyRotationTurret(int);

        /**
         * Notifica del cambio de la vida actual del tanque.
         */
        void                    notifyCurrentLive(int);

        /**
         * Notifica del cambio de la vida máxima del tanque.
         */
        void                    notifyMaxLive(int);

        /**
         * Notifica del cambio de la energía actual del tanque.
         */
        void                    notifyCurrentMana(int);

        /**
         * Notifica del cambio de la energía máxima del tanque.
         */
        void                    notifyMaxMana(int);

        /**
         * Notifica del cambio de las bonificaciones de los atributos @b _base_live_regeneration,
         * @b _base_mana_regeneration, @b _base_armor, @b _base_speed.
         */
        void                    notifyEffectChange();

        /**
         * Notifica del cambio de estado del tanque.
         * @see Tank_Status
         */
        void                    notifyStatus(Tank_Status);

        /**
         * Notifica cuando el tanque muere.
         */
        void                    notifyDead(ITank* killer);

        /**
         * Notifica cuando el tanque renace.
         */
        void                    notifyReborn();

        /**
         * Notifica el tiempo que falta para que el tanque "reviva".
         */
        void                    notifyTimeToReborn(int);

        /**
         * Notifica el cambio de experiencia (_current_experience, _next_level_experience)
         */
        void                    notifyExperience(int, int);

        /**
         * Notifica del cuando el tanque sube de nivel
         */
        void                    notifyLevelUp(int);

        /**
         * Notifica cuando una habilidad es upgradeada.
         */
        void                    notifySkillLevelUp(int skill, int level);

    protected:

        /**
         * Este es el método para dibujar el tanque en los clientes amigos
         */
        virtual void            paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

        /**
         * Este es el método para dibujar el tanque en los clientes enemigos
         */
        virtual void            paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

        /**
         * Se utiliza para el lazo de eventos del tanque.
         */
        virtual void            gameLoop() = 0;

        /**
         * Mueve el tanque a su nueva posición dependiendo del valor de los atributos @b _move_forward y @b _move_backward.
         * @see setMoveForward(bool value)
         * @see isMoveForward()
         * @see setMoveBackward(bool value)
         * @see isMoveBackward()
         */
        virtual void            move();

        /**
         * Rota el tanque a su nuevo ángulo dependiendo del valor de los atributos @b _rotate_right y @b _rotate_left.
         * @see setRotateRight(bool value)
         * @see isRotateRight()
         * @see setRotateLeft(bool value)
         * @see isRotateLeft()
         */
        virtual void            rotate();

        /**
         * Verifica que la habilidad sea activa, lista para ejecutarse y que la energía alcance para ejecutarla.
         * Ejecuta la habilidad posicionada en @b index si el atributo @b _status es diferente de @b STUNNED ó @b DEAD.
         * Además verifica que modifica la habilidad lanzada por las otras (si modifican habilidades, si estan lanzadas o son pasiva).
         * @see ISkill
         */
        virtual void            executeSkill(int index);

        /**
         * Establece el estado del tanque en @b DEAD, además de inciar en conteo
         * para el renacimiento en @b _current_dead_time.
         * @see Tank_Status;
         * @see deadAnimation()
         */
        virtual void            dead();

        /**
         * Cambia a los valores iniciales los atributos del tanque excepto
         * @b _curretn_experience, @b _level y @b _next_level_experience.
         */
        virtual void            reborn();

        QRectF                  _default_rect;

        //propiedades del jugador
        QString                 _nick;
        int                     _rank;
        ColorTeam               _color_team;

        //estado de movimiento del tanque
        bool                    _move_forward;
        bool                    _move_backward;

        //estado de rotación del tanque
        bool                    _rotate_left;
        bool                    _rotate_right;

        //vida
        double                  _current_live;
        double                  _max_live;

        //energía
        double                  _current_mana;
        double                  _max_mana;

        //regeneración de vida por segundo
        double                  _base_live_regeneration;

        //regeneración de mana por segundo
        double                  _base_mana_regeneration;

        //armadura
        double                  _base_armor;

        //velocidad
        double                  _base_speed;

        //tiempo para revivir en segundos
        int                     _dead_time;
        int                     _current_dead_time;

        //ángulo del tanque
        double                  _angle;

        //ángulo de la torreta
        double                  _turret_rotation;

        //define si es amigo o enemigo
        bool                    _is_enemy;

        //las cuatro habilidades
        ISkillFactory*          _skill_1;
        ISkillFactory*          _skill_2;
        ISkillFactory*          _skill_3;

        //listas de aliados, enemigos y otros
        QList<ITank*>           _others_tanks;
        QList<ITank*>           _allies;
        QList<ITank*>           _enemie;

        //estado del tanque
        Tank_Status             _status;

        //barra de información
        InfoBar*                _info_bar;

        ITank*                  _last_tank_cause_damage;

        QGraphicsPixmapItem*    _mini_map_tank;

        //circles of powers
        CircleOfPower*          _circle_of_power;
        QList<CircleOfPower*>   _others_circles_of_powers;

    private:

        /**
         * Aumenta en 1 el atributo @b _level. Le resta a @b _current_experience el valor de
         * @b _next_level_experience. El valor de @ _next_level_experience se iguala a 100* el valor del
         * atributo @b _level. Adiciona los @b incrementos a los restantes atributos.
         * @see notifyLevelUp(int)
         */
        void                    levelUp();

        QList<QPixmap>          _animation_level_up;
        int                     _frame_animation_level_up;
        bool                    inLevelUpAnimation();
        void                    levelUpAnimation();

        QList<QPixmap>          _animation_dead;
        int                     _frame_animation_dead;
        bool                    inDeadAnimation();
        void                    deadAnimation();

        //indica que habilidades se mandaron a ejecutar
        bool                    _hurled_skills[3];

        bool                    _ready_to_execute_skill;
        int                     _counter_delay_beteen_skills;

        int                     _current_experience;
        int                     _next_level_experience;
        int                     _level;
        int                     _updates_available;

        //información del tanque
        TankInfo                _tank_info;

        //areas obstruidas en el terreno
        QList<QPainterPath>     _obstructed_areas;

        bool                    collideWithSomeThing();

        bool                    collidesWithObstructedAreas();

        bool                    isOutOfCorner();

        QList<Effect*>          _effects;

        int                     _counter_millisec;

        SoundEngine*            _sound_explode;
        SoundEngine*            _sound_level_up;

        ITank*                  _principal_reference;

        int                     _corner[2];
};

class GT_CORESHARED_EXPORT ITankFactory
{
    public:

        virtual                         ~ITankFactory(){}

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        virtual QString                 pluginName() = 0;
        virtual int                     gameVersion() = 0;

        TankInfo                        tankInfo(){return _tank_info;}

        //crea una instancia del tanque
        virtual ITank*                  createInstance(QString nick, int rank, ColorTeam color_team) = 0;

    protected:

        TankInfo                        _tank_info;

};

#define ITankFactory_iid "xavier.gt.itankfactory"
Q_DECLARE_INTERFACE(ITankFactory,ITankFactory_iid)

#endif // ITANK_H

