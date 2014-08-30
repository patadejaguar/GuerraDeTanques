#ifndef CIRCLEOFPOWER_H
#define CIRCLEOFPOWER_H

#include "gt_core_global.h"
#include "iobject.h"

class ITank;

class GT_CORESHARED_EXPORT CircleOfPower: public IObject
{
        Q_OBJECT

    public:

        CircleOfPower(bool edit_mode = false);

        void                    setTank(ITank* tank);
        void                    initObject();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

        inline QString          nameForDependency() const{return "xavier_object_circle_of_power";}

        virtual void            save(QDataStream &stream);

        virtual void            loadFromData(QDataStream &stream);

        QList<QWidget*>         getControls();

        int                     live() const;

        void                    addDamage(int damage, ITank *tank);

        bool                    isEnemy();

        bool                    isActive();

        int                     team() const;

        int                     player() const;

        ITank*                  tank();

    public slots:

        void                    setAttribute();

    signals:

        void                    notifyDesactive(ITank*);

    protected:

        void                    gameLoop();

        void                    paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private slots:

        void                    tankReborn();

    private:

        int                     _team;

        int                     _player;

        ITank*                  _tank;

        ITank*                  _last_tank_cause_live_changed;

        double                  _live;

        double                  _max_live;

        int                     _tank_direction;

        int                     _index_of_graphics_live;

        QList<QPixmap>          _graphics_live;
        QPixmap                 _pixmap_tank_direction;

        bool                    _tank_activated;
        QList<Effect*>          _effect;
        QList<QPixmap>          _pixmap_effect;
};

class GT_CORESHARED_EXPORT CircleOfPowerFactory: public QObject, public IObjectFactory
{
        Q_OBJECT
        Q_INTERFACES(IObjectFactory)

    public:

        CircleOfPowerFactory()
        {
            _preview = QPixmap(":/gt/sprites/preview_circle_of_power.png");
        }

        QPixmap getPreview(){return _preview;}

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        inline QString          pluginName() const          {return "Círculo de poder";}
        inline QString          nameForDependency() const   {return "xavier_object_circle_of_power";}
        inline int              gameVersion() const         {return 1;}

        inline QString          description() const         {return "Proporcina energía al tanque. Regeneración de vida y mana.";}

        //crea una instancia del objeto
        inline virtual IObject*         createInstance(bool edit_mode = false){return new CircleOfPower(edit_mode);}
};

#endif // CIRCLEOFPOWER_H
