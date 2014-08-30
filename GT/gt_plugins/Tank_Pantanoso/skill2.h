#ifndef SKILL2_H
#define SKILL2_H

#include <QGraphicsScene>
#include <QMediaPlaylist>

#include "iskill.h"
#include "utilities.h"
#include "soundengine.h"

class Effect;

class Skill2 : public ISkill
{
        Q_OBJECT

    public:

        explicit                Skill2(ITank *tank, int live_regen, QList<QPixmap> pixmaps, SoundEngine* sound_engine);

        ~Skill2();

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

        QList<QPixmap>          _pixmaps;

        int                     _milliseconds;

        QMap< ITank*, Effect*>  _effects;

        SoundEngine*            _sound_engine;
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

        int                     _live_regen;

        SoundEngine*            _sound_engine;

        QList<QPixmap>          _pixmaps;

};

#endif // SKILL2_H
