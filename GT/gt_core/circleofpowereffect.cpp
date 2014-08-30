#include "circleofpowereffect.h"

CircleOfPowerEffect::CircleOfPowerEffect(ITank *creator_tank, ITank *target_tank, double bonus_live_regeneration, double bonus_mana_regeneration, double bonus_armor, double bonus_speed, bool stunned, bool p_forever, int time, QList<QPixmap> sprite, qreal index_z_value)
    :Effect(creator_tank, target_tank, bonus_live_regeneration, bonus_mana_regeneration, bonus_armor, bonus_speed, stunned, p_forever, time, sprite, index_z_value)
{
}

void CircleOfPowerEffect::setTimeZero()
{
    _time = 0;
    _forever = false;
}

void CircleOfPowerEffect::advance(int phase)
{
    if(!phase)
        return;

    if(++_step >= _sprite.size())
        _step = 0;
}
