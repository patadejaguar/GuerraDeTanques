#include "object.h"

Object::Object(bool edit_mode):IObject(edit_mode)
{

    _width = 24;

    _height = 24;

    _experience = 28;

    _current_cooldown = 0;

    _one_second = 0;

    _cooldown = 40;

    _tileset = getListOfPixmapFromStripImage(":/xavier_object_tablet_of_experience/tablet_of_experience.png",_width);

    _explode = getListOfPixmapFromStripImage(":/xavier_object_tablet_of_experience/explode.png",55);

    _mini_map_view->setPixmap(QPixmap(":/xavier_object_tablet_of_experience/mini_map_object.png"));

    _sound = new SoundEngine(QUrl("qrc:/xavier_object_tablet_of_experience/rune.mp3"), MECH_MAX_DISTANCE_SOUND, this);
}

QPainterPath Object::shape() const
{
    QPainterPath path;
    path.addRect(-_width/2, -_height/2, _width, _height);
    return path;
}

QRectF Object::boundingRect() const
{
    return QRectF(-_width/2, -_height/2, _width, _height);
}

void Object::save(QDataStream &stream)
{
    IObject::save(stream);
    stream << _experience;
    stream << _cooldown;
}

void Object::loadFromData(QDataStream &stream)
{
    IObject::loadFromData(stream);
    stream >> _experience;
    stream >> _cooldown;
}

QList<QWidget *> Object::getControls()
{
    QSpinBox *experience = new QSpinBox();
    experience->setObjectName("experience");
    experience->setMaximum(1000);
    experience->setValue(_experience);
    connect(experience,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_experience = new Control("Bonificación de experiencia",experience);

    QSpinBox *cooldown = new QSpinBox();
    cooldown->setSuffix(" seg");
    cooldown->setObjectName("cooldown");
    cooldown->setMaximum(1000);
    cooldown->setValue(_cooldown);

    connect(cooldown,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_cooldown = new Control("Tiempo de reaparición",cooldown);

    QList<QWidget*> controls = IObject::getControls();
    controls.append(c_experience);
    controls.append(c_cooldown);

    return controls;
}

void Object::setAttribute()
{
    IObject::setAttribute();
    QObject* widget = sender();
    if(widget->objectName() == "experience")
        _experience = ((QSpinBox*)widget)->value();
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

        _width = _height = 24;

        if(_frame_explode <= 15)
        {
            ++_frame_explode;

            _width = _height = 55;
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
                tank->addExperience(_experience);
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
        painter->drawPixmap(-_width/2,-_height/2,_width,_height,_explode.at(_frame_explode));
}

