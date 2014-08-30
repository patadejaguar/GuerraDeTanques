#ifndef SKILL3_H
#define SKILL3_H

#include <QGraphicsScene>

#include "iskill.h"
#include "singleanimation.h"
#include "utilities.h"
#include "soundengine.h"

class ITank;
class Effect;

class Skill3 : public ISkill
{
        Q_OBJECT

    public:

        explicit                Skill3(ITank *tank, int damage, int slow_down_time, QPointF turret_pos, int turret_angle, QList<QPixmap> pixmaps_shot);

    protected:

        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    gameLoop();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

    private:

        void                    explod();

        int                     _damage;

        int                     _slow_down_time;

        bool                    _explode;

        int                     _step;

        int                     _angle;

        double                  _velocity;

        int                     _distance;

        QList<QPixmap>          _pixmaps_shot;

        Effect*                  _effect;

        SoundEngine*            _sound;
};

class Skill3Factory: public ISkillFactory
{
    public:

        Skill3Factory(ITank* tank, SkillInfo skill_info);

        QString                 getDetails();

        void                    updateAttributes();

    protected:

        ISkill*                 createSkill();

    private:

        int                     _damage;

        int                     _slow_down_time;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps_shot;

        QList<QPixmap>          _pixmaps_shot_fire;
};

#endif // SKILL3_H
