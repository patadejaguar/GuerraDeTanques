#ifndef SKILL2_H
#define SKILL2_H

#include <QGraphicsScene>

#include "iskill.h"
#include "utilities.h"
#include "soundengine.h"
#include "drains.h"

class Effect;

class Skill2 : public ISkill
{
        Q_OBJECT

    public:

        explicit                Skill2(ITank *tank, int mana_drain, QList<QPixmap> pixmaps_drain);

    protected:

        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    gameLoop();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

    private:

        QList<QPixmap>          _pixmaps_drain;

        int                     _mana_drain;

};


class Skill2Factory: public ISkillFactory
{
    public:

        Skill2Factory(ITank* tank, SkillInfo skill_info);

        QString                 getDetails();

        void                    updateAttributes();

    protected:

        ISkill*                 createSkill();

    private:

        int                     _mana_drain;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps_drain;

};

#endif // SKILL2_H
