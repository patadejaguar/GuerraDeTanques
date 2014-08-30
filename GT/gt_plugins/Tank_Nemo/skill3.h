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

        explicit                Skill3(ITank *tank, int bonus_armor, int time_effect, QList<QPixmap> pixmaps_armor);

    protected:

        void                    paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    gameLoop();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

    private slots:

        void                    setPosition(QPointF pos);
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

        int                     _bonus_armor;

        int                     _time_effect;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps;

};

#endif // SKILL3_H
