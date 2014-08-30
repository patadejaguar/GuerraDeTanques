#ifndef MYEFFECT_H
#define MYEFFECT_H

#include "effect.h"

class MyEffect : public Effect
{
        Q_OBJECT

    public:

        MyEffect(ITank* creator_tank, ITank* target_tank, double bonus_live_regeneration, double bonus_mana_regeneration, double bonus_armor, double bonus_speed, bool stunned, bool p_forever, int time, QList<QPixmap> sprite = QList<QPixmap>(), qreal index_z_value = +0.1);

    public slots:

        void    setTimeZero();

        void    advance(int phase);
};

#endif // MYEFFECT_H
