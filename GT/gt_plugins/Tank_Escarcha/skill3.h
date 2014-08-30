#ifndef SKILL3_H
#define SKILL3_H

#include <QGraphicsScene>

#include "iskill.h"
#include "utilities.h"
#include "soundengine.h"

class Effect;

class Skill3 : public ISkill
{
        Q_OBJECT

    public:

        explicit                Skill3(ITank *tank, int time_effect, QList<QPixmap> pixmaps, QList<QPixmap> frozen_effect);

    protected:

        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    gameLoop();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

    private slots:

        void                    setPosition(QPointF pos);

    private:

        int                     _step;

        int                     _time_effect;

        QList<QPixmap>          _pixmaps;

        QList<QPixmap>          _frozen_effect;

        QMap< ITank*, Effect*>  _effects;
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

        int                     _time_effect;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps;

        QList<QPixmap>          _frozen_effect;

};

#endif // SKILL3_H
