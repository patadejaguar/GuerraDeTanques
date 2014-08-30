#ifndef SKILL1_H
#define SKILL1_H

#include <QGraphicsScene>

#include "iskill.h"
#include "singleanimation.h"
#include "utilities.h"
#include "soundengine.h"

class ITank;

class Skill1 : public ISkill
{
        Q_OBJECT

    public:

        explicit                Skill1(ITank *tank, int damage, QPointF turret_pos, int turret_angle, QList<QPixmap> pixmap_shot);

    protected:

        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    gameLoop();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

    private:

        void                    explod();

        int                     _damage;

        bool                    _explode;

        int                     _step;

        int                     _angle;

        double                  _velocity;

        int                     _distance;

        QList<QPixmap>          _pixmaps_shot;

        SoundEngine*            _sound;
};

class Skill1Factory: public ISkillFactory
{
    public:

        Skill1Factory(ITank* tank, SkillInfo skill_info);

        QString                 getDetails();

        void                    updateAttributes();

    protected:

        ISkill*                 createSkill();

    private:

        int                     _damage;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps_shot;
};

#endif // SKILL1_H
