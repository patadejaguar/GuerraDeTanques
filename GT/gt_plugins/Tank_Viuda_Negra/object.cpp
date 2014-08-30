#include "object.h"
#include "itank.h"

Object::Object(QList<QPixmap> tile_set, int time, ITank *tank, bool edit_mode):IObject(edit_mode)
{
    _step = 0;

    _width = 100;

    _height = 100;

    _time_effect = time;

    _time_live = 120000;

    _activated = false;

    _tank = tank;

    _tileset = tile_set;

    _mini_map_view->setPixmap(QPixmap(":/xavier_tank_viuda_negra/sprites/mini_map_tramp.png"));

    _mini_map_view->setZValue(_tank->miniMapTank()->zValue()-0.1);

    _sound = new SoundEngine(QUrl("qrc:/xavier_tank_viuda_negra/sounds/tramp.mp3"), MECH_MAX_DISTANCE_SOUND, this);
}

Object::~Object()
{
}

QPainterPath Object::shape() const
{
    QPainterPath path;
    path.addRect(-10, -10, 20, 20);
    return path;
}

QRectF Object::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void Object::gameLoop()
{    
    _mini_map_view->setVisible(!_tank->isEnemy());

    if(_activated)
    {
        if(_step < _tileset.size()-2)
            ++_step;
        _time_effect-=TICK;
        if(_time_effect <= 0)
        {
            _tank->miniMapTank()->scene()->removeItem(_mini_map_view);
            _tank->scene()->removeItem(this);
            deleteLater();
        }
    }
    else
    {
        _time_live-=TICK;
        if(_time_live <= 0)
        {
            _tank->miniMapTank()->scene()->removeItem(_mini_map_view);
            _tank->scene()->removeItem(this);
            deleteLater();
        }
        else
        {
            foreach(ITank* tank, _tank->getEnemie())
            {
                if(tank->getStatus() != DEAD && collidesWithItem(tank))
                {
                    Effect* effect_tank = new Effect(tank, tank, 0, 0, 0, -tank->getTotalSpeed(), false, false, _time_effect, QList<QPixmap>());
                    tank->addEffect(effect_tank);

                    setPos(tank->pos());
                    setZValue(tank->zValue()+0.1);

                    _sound->playSound2D(tank->pos(),tank->principalReference()->pos(), Configuration::volumeSound());

                    _activated = true;

                    _time_effect = _time_effect*1000;

                    return;
                }
            }
        }
    }
}

void Object::paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Object::paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(_tank->isEnemy() && !_activated)
        return;
    painter->drawPixmap(-_width/2,-_height/2,_width,_height,_tileset.at(_step));
}


