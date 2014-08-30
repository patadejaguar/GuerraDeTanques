#ifndef CIRCLEOFPOWEREFFECT_H
#define CIRCLEOFPOWEREFFECT_H

#include "effect.h"

class CircleOfPowerEffect : public Effect
{
    public:
        CircleOfPowerEffect(ITank* creator_tank, ITank* target_tank, double bonus_live_regeneration, double bonus_mana_regeneration, double bonus_armor, double bonus_speed, bool stunned, bool p_forever, int time, QList<QPixmap> sprite = QList<QPixmap>(), qreal index_z_value = +0.1);

    public slots:

        void    setTimeZero();

        void    advance(int phase);
};

#endif // CIRCLEOFPOWEREFFECT_H
