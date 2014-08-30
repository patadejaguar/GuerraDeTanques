#include "object.h"


Object::Object(QList<QPixmap> tile_set, QList<QPixmap> animation, bool edit_mode):IObject(edit_mode)
{
    _width = 60;

    _height = 60;

    _bonus = 30;

    _time = 15;

    _cooldown = 40;

    _current_cooldown = 0;

    _one_second = 0;

    _tileset = tile_set;

    _animation = animation;

    _mini_map_view->setPixmap(QPixmap(":/xavier_object_rune_of_speed/mini_map_object.png"));

    _sound = new SoundEngine(QUrl("qrc:/xavier_object_rune_of_speed/rune.mp3"), MECH_MAX_DISTANCE_SOUND, this);

    _effect_animation = getListOfPixmapFromStripImage(":/xavier_object_rune_of_speed/effect_animation.png",85);
}

Object::~Object()
{
}

QPainterPath Object::shape() const
{
    QPainterPath path;
    path.addRect(-12, -12, 24, 24);
    return path;
}

QRectF Object::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void Object::save(QDataStream &stream)
{
    IObject::save(stream);
    stream << _bonus;
    stream << _time;
    stream << _cooldown;
}

void Object::loadFromData(QDataStream &stream)
{
    IObject::loadFromData(stream);
    stream >> _bonus;
    stream >> _time;
    stream >> _cooldown;
}

QList<QWidget *> Object::getControls()
{
    QSpinBox *bonus = new QSpinBox();
    bonus->setObjectName("bonus");
    bonus->setMaximum(1000);
    bonus->setValue(_bonus);
    connect(bonus,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_bonus = new Control("Bonificación de velocidad", bonus);

    QSpinBox *time = new QSpinBox();
    time->setSuffix(" seg");
    time->setObjectName("time");
    time->setMinimum(1);
    time->setMaximum(1000);
    time->setValue(_time);
    connect(time,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_time = new Control("Tiempo de efecto", time);

    QSpinBox *cooldown = new QSpinBox();
    cooldown->setSuffix(" seg");
    cooldown->setObjectName("cooldown");
    cooldown->setMinimum(1);
    cooldown->setMaximum(1000);
    cooldown->setValue(_cooldown);
    connect(cooldown,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_cooldown = new Control("Tiempo de reaparición", cooldown);

    QList<QWidget*> controls = IObject::getControls();
    controls.append(c_bonus);
    controls.append(c_time);
    controls.append(c_cooldown);

    return controls;
}

void Object::setAttribute()
{
    IObject::setAttribute();
    QObject* widget = sender();
    if(widget->objectName() == "bonus")
        _bonus = ((QSpinBox*)widget)->value();
    else if(widget->objectName() == "time")
        _time = ((QSpinBox*)widget)->value();
    else if(widget->objectName() == "cooldown")
        _cooldown = ((QSpinBox*)widget)->value();
}

void Object::gameLoop()
{
    if(_current_cooldown > 0)
    {
        _one_second += TICK;
        if(_one_second >= 1000)
        {
            _one_second -= 1000;
            --_current_cooldown;
        }

        _width = _height = 60;

        if(_frame_explode <= 15)
        {
            ++_frame_explode;

            _width = _height = 60;
        }

        _mini_map_view->setVisible(false);
    }
    else
    {
        _mini_map_view->setVisible(true);

        _step = (_step + 1) % _tileset.size();

        foreach(ITank* tank, _tanks)
        {
            if(tank->getStatus() != DEAD && collidesWithItem(tank))
            {
                Effect* effect_tank = new Effect(tank, tank, 0, 0, 0, _bonus, false, false, _time, _effect_animation, -0.1);
                tank->addEffect(effect_tank);

                _current_cooldown = _cooldown;
                _frame_explode = 0;

                _sound->playSound2D(tank->pos(),tank->principalReference()->pos(), Configuration::volumeSound());
            }
        }
    }
}

void Object::paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    IObject::paintForEditMode(painter, option, widget);
    painter->drawPixmap(-_width/2,-_height/2,_width,_height,_tileset.at(_step));
}

void Object::paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(_current_cooldown == 0)
        painter->drawPixmap(-_width/2,-_height/2,_width,_height,_tileset.at(_step));
    else if(_frame_explode < 15)
        painter->drawPixmap(-_width/2,-_height/2,_width,_height,_animation.at(_frame_explode));
}

