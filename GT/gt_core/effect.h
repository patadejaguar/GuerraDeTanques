#ifndef EFFECT_H
#define EFFECT_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QList>

#include "gt_core_global.h"

class ITank;

class GT_CORESHARED_EXPORT Effect: public QGraphicsObject
{
    public:

        Effect(ITank* creator_tank, ITank* target_tank, double bonus_live_regeneration, double bonus_mana_regeneration, double bonus_armor, double bonus_speed, bool stunned, bool p_forever, int time, QList<QPixmap> sprite = QList<QPixmap>(), qreal index_z_value = +0.1);

        ITank*                  creatorTank();

        ITank*                  targetTank();

        double                  bonusLiveRegeneration() const;
        void                    setBonusLiveRegeneration(int bonus_live_regeneration);

        double                  bonusManaRegeneration() const;
        void                    setBonusManaRegeneration(int bonus_mana_regeneration);

        double                  bonusArmor() const;
        void                    setBonusArmor(int bonus_armor);

        double                  bonusSpeed() const;
        void                    setBonusSpeed(int bonus_speed);

        bool                    isForever();

        int                     time();

        bool                    nextTime();

        virtual void            paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        bool                    stunned() const;
        void                    setStunned(bool stunned);

    public slots:

        void                    advance(int phase);

        virtual void            destroy();

    protected:

        ITank*                  _creator_tank;
        ITank*                  _target_tank;
        double                  _bonus_live_regeneration;
        double                  _bonus_mana_regeneration;
        double                  _bonus_armor;
        double                  _bonus_speed;
        bool                    _stunned;

        QRectF                  boundingRect() const;

    private slots:

        void                    setPosition(QPointF pos);

    protected:

        /**
         * Es verdadero cuando cuando el tiempo de vida es eterno.
         */
        bool                    _forever;

        /**
         * Tiempo de vida en segundos
         */
        int                     _time;

        /**
         * Tira de imagenes
         */
        QList<QPixmap>          _sprite;

        /**
         * Paso para la animación
         */
        int                     _step;

};
#endif // EFFECT_H
