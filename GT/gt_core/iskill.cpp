#include "iskill.h"
#include "itank.h"

ISkill::ISkill(ITank *tank):
    QGraphicsObject()
{
    _tank = tank;
}

void ISkill::destroy()
{
    scene()->removeItem(this);
    deleteLater();
}

ITank *ISkill::getTank()
{
    return _tank;
}

void ISkill::advance(int phase)
{
    if(!phase)
        return;

    gameLoop();
}

void ISkill::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(_tank->isEnemy())
        paintForEnemies(painter,option,widget);
    else
        paintForFriends(painter,option,widget);
}

ISkillFactory::ISkillFactory(ITank *tank, SkillInfo skill_info, int cooldown, int mana_cost)
{
    _tank = tank;
    _skill_info = skill_info;
    _cooldown = cooldown;
    _mana_cost = mana_cost;
    _level = 0;
    _current_cooldown = 0;
    _full_cooldown = cooldown;
    _skill_info._definitive ? _tank_level_request_for_update = 4 : _tank_level_request_for_update = 1;
    _active = false;
}

SkillInfo ISkillFactory::skillInfo() const
{
    return _skill_info;
}

int ISkillFactory::getLevel() const
{
    return _level;
}

QString ISkillFactory::getName()
{
    return _skill_info._name;
}

QString ISkillFactory::getDescription()
{
    return _skill_info._description;
}

QPixmap ISkillFactory::getPreview()
{
    return _skill_info._preview;
}

bool ISkillFactory::isReadyToExecute()
{
    return _active && _current_cooldown == 0 && _mana_cost <= _tank->currentMana();
}

void ISkillFactory::executeSkill()
{
    int current_mana = _tank->currentMana() - _mana_cost;

    if(current_mana >= 0)
    {
        _tank->setCurrentMana(current_mana);
        _current_cooldown = _full_cooldown = _cooldown;

        ISkill * skill = createSkill();

        if(skill != NULL)
        {
            addSkillToScene(skill);

            updateCooldown();
        }
    }
}

void ISkillFactory::active()
{
    _active = true;

    levelUp();

    emit notifyInit();
}

bool ISkillFactory::isActive()
{
    return _active;
}

void ISkillFactory::levelUp()
{
    if(!_active)
    {
        active();
        return;
    }

    ++_level;

    if(_skill_info._definitive)
    {
        if(_level < 3)
            _tank_level_request_for_update += 4;
        else
            _tank_level_request_for_update = -1;
    }
    else
    {
        if(_level < 4)
            _tank_level_request_for_update += 2;
        else
            _tank_level_request_for_update = -1;
    }
    if(_level > 1)
        updateAttributes();
}

bool ISkillFactory::isReadyForUpdate()
{
    return _tank->getUpdatesAvailablesForSkills() > 0 && _tank_level_request_for_update <= _tank->getLevel() && _tank_level_request_for_update != -1;
}

bool ISkillFactory::inCooldown()
{
    return _current_cooldown != 0;
}

int ISkillFactory::getManaCost()
{
    return _mana_cost;
}

int ISkillFactory::getTankLevelForUpdate()
{
    return _tank_level_request_for_update;
}

void ISkillFactory::updateCooldown()
{
    _current_cooldown-= TICK;
    if(_current_cooldown <= 0)
        resetCooldown();
    emit notifyCurrentPercentCooldown(getPercent(_full_cooldown, _current_cooldown));
}

void ISkillFactory::resetCooldown()
{
    _current_cooldown = 0;
    emit notifyCooldownFinished();
}

ISkill* ISkillFactory::createSkill()
{
    return NULL;
}

void ISkillFactory::addSkillToScene(ISkill *skill)
{
    _tank->scene()->addItem(skill);
}
bool ISkillFactory::isDefinitive()
{
    return _skill_info._definitive;
}
