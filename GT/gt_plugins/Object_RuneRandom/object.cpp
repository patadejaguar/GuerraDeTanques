#include "object.h"
#include "randomnumbermannager.h"

Object::Object(bool edit_mode):IObject(edit_mode)
{
    _active_rune = 0;

    _width = 60;

    _height = 60;

    _bonus = 30;

    _time = 15;

    _cooldown = 40;

    _current_cooldown = 0;

    _one_second = 0;

    for (int i = 1; i < 5; ++i)
    {
        _animations.append(getListOfPixmapFromStripImage(QString(":/xavier_object_rune_random/animation_%1.png").arg(i), _width));

        _explodes.append(getListOfPixmapFromStripImage(QString(":/xavier_object_rune_random/explode_%1.png").arg(i), 60));

        _mini_map_views.append(QPixmap(QString(":/xavier_object_rune_random/mini_map_object_%1.png").arg(i)));

        _effect_animations.append(getListOfPixmapFromStripImage(QString(":/xavier_object_rune_random/effect_animation_%1.png").arg(i), 85));

        _sounds.append(new SoundEngine(QUrl(QString("qrc:/xavier_object_rune_random/sound_%1.mp3").arg(i)), MECH_MAX_DISTANCE_SOUND, this));
    }

    _tileset = _animations.at(_active_rune);    
    _mini_map_view->setPixmap(QPixmap(QString(":/xavier_object_rune_random/mini_map_object_0.png")));

    _pixmap_for_edit_mode = QPixmap(":/xavier_object_rune_random/preview.png");
}

Object::~Object()
{
}

void Object::initObject()
{
    _active_rune = RandomNumberMannager::getMaxRandomNumber(3);
    _tileset = _animations.at(_active_rune);
    _mini_map_view->setPixmap(_mini_map_views.at(_active_rune));
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
    Control *c_bonus = new Control("Bonificación", bonus);

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
        if(!_mini_map_view->isVisible())
        {
            _mini_map_view->setVisible(true);
            initObject();
        }

        _step = (_step + 1) % _tileset.size();

        foreach(ITank* tank, _tanks)
        {
            if(tank->getStatus() != DEAD && collidesWithItem(tank))
            {
                if(_active_rune == 0)
                {
                    Effect* effect_tank = new Effect(tank, tank, _bonus, 0, 0, 0, false, false, _time, _effect_animations.at(_active_rune), -0.1);
                    tank->addEffect(effect_tank);
                }
                else if(_active_rune == 1)
                {
                    Effect* effect_tank = new Effect(tank, tank, 0, _bonus, 0, 0, false, false, _time, _effect_animations.at(_active_rune), -0.1);
                    tank->addEffect(effect_tank);
                }
                else if(_active_rune == 2)
                {
                    Effect* effect_tank = new Effect(tank, tank, 0, 0, _bonus, 0, false, false, _time, _effect_animations.at(_active_rune));
                    tank->addEffect(effect_tank);
                }
                else if(_active_rune == 3)
                {
                    Effect* effect_tank = new Effect(tank, tank, 0, 0, 0, _bonus, false, false, _time, _effect_animations.at(_active_rune), -0.1);
                    tank->addEffect(effect_tank);
                }

                _current_cooldown = _cooldown;
                _frame_explode = 0;

                _sounds.at(_active_rune)->playSound2D(tank->pos(),tank->principalReference()->pos(), Configuration::volumeSound());
            }
        }
    }
}

void Object::paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    IObject::paintForEditMode(painter, option, widget);
    painter->drawPixmap(-_width/2,-_height/2, _width,_height, _pixmap_for_edit_mode);
}

void Object::paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(_current_cooldown == 0)
        painter->drawPixmap(-_width/2,-_height/2, _width,_height, _tileset.at(_step));
    else if(_frame_explode < 15)
        painter->drawPixmap(-_width/2,-_height/2, _width,_height, _explodes.at(_active_rune).at(_frame_explode));
}

