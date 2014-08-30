#ifndef ISKILL_H
#define ISKILL_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>
#include <QBitmap>

#include "gt_core_global.h"

struct SkillInfo
{
        QString     _name;
        QString     _description;
        QString     _details;
        QPixmap     _preview;
        bool        _definitive;
};

class ITank;

class GT_CORESHARED_EXPORT ISkill : public QGraphicsObject
{
        Q_OBJECT

    public:
        /**
         * Crea la habilidad en el tanque @b tank. Si el atributo @b ultimate es igual a @b true,
         * esta habilidad se considera como la definitiva.
         * @see ITank
         */
        ISkill(ITank *tank);
        virtual ~ISkill(){}


        /**
         * Retorna el tanque para el que fue creada esta habilidad.
         * @see Itank
         */
        ITank*                  getTank();

        virtual void            paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    public slots:

        void                    advance(int phase);

        /**
         * Elimina la habilidad de la escena y la destruye.
         */
        virtual void            destroy();

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
         * Se utiliza para el lazo de eventos de las habilidades
         */
        virtual void            gameLoop() = 0;

        //variables que se usan para no calcularlas cada vez que se pregunta por colisiones
        QPainterPath            _shape;
        QRectF                  _bounding_rect;

        //retorna el tanque que creo la habilidad
        ITank *                 _tank;

};

/**
 * Esta clase se usa para crear las habilidades de los tanques.
 */
class GT_CORESHARED_EXPORT ISkillFactory: public QObject
{
        Q_OBJECT

    public:

        ISkillFactory(ITank *tank, SkillInfo skill_info, int cooldown, int mana_cost);
        virtual ~ISkillFactory(){}

        SkillInfo               skillInfo() const;

        /**
         * Inicializa la habilidad para poder utilizarla.
         */
        virtual void            active();

        bool                    isActive();

        /**
         * Retorna el valor del atributo @b _level;
         */
        int                     getLevel() const;

        /**
         * Retorna el nombre de la habilidad.
         */
        QString                 getName();

        /**
         * Retorna la descripción de la habilidad.
         */
        QString                 getDescription();

        /**
         * Retorna la descripción detallada de la habilidad.
         */
        virtual QString         getDetails() = 0;

        /**
         * Retorna la vista previa de la habilidad 75 x 75.
         */
        QPixmap                 getPreview();

        /**
         * Devuelve verdadero en caso de que se pueda ejecutar la habilidad.
         */
        bool                    isReadyToExecute();

        /**
         * Ejecuta la habilidad.
         * @see createSkill(Itank *tank, ...)
         */
        void                    executeSkill();

        /**
         * Adiciona un nivel a la habilidad mejorando sus atributos.
         * @see level()
         * @see updateAttributes()
         * @see getTankLevelRequestForUpdate()
         */
        void                    levelUp();

        /**
         * Retorna verdadero en caso de que la habilidad se pueda mejorar.
         */
        bool                    isReadyForUpdate();

        /**
         * Mejora los atributos de la habilidad por nivel.
         */
        virtual void            updateAttributes() = 0;

        /**
         * Retorna verdadero si no se puede usar la habilidad por estar cargandoce.
         */
        bool                    inCooldown();

        /**
         * Retorna el costo de mana
         */
        int                     getManaCost();

        /**
         * Retorna el siguiente valor requerido por el tanque para mejorar la habilidad.
         */
        int                     getTankLevelForUpdate();

        /**
         * Retorna el valor del atributo @b _definitive.
         */
        bool                    isDefinitive();

        /**
         * Decrementa el valor del atributo @b _current_cooldown hasta llegar a 0.
         * @see getCurrentCooldown()
         * @see notifyCooldown(int)
         */
        void                    updateCooldown();

        /**
         * Reinicia el cooldown dejando la habilidad lista para ejecutarce.
         */
        void                    resetCooldown();

    signals:

        /**
         * Notifica cuando la habilidad es habilitada. Esto sucede solo una vez.
         */
        void                    notifyInit();

        /**
         * Notifica cuando el atributo @b _current_cooldown, lo que significa
         * que la habilidad está lista para ejecutarse.
         */
        void                    notifyCooldownFinished();

        /**
         * Notifica del cooldown actual en milisegundos (cada 100 milisegundos)
         */
        void                    notifyCurrentPercentCooldown(int);

    protected:

        /**
         * Función que crea la habilidad y la adiciona a la escena para que se pueda ejecutar
         * el método @b ISkill::gameLoop().
         */
        virtual ISkill *        createSkill();

        void                    addSkillToScene(ISkill* skill);

    protected:

        ITank*                  _tank;

        int                     _cooldown;

        int                     _full_cooldown;

        int                     _current_cooldown;

        int                     _mana_cost;

    private:

        SkillInfo               _skill_info;

        bool                    _active;

        int                     _level;

        int                     _tank_level_request_for_update;
};
#endif // ISKILL_H
