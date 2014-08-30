#include "circleofpower.h"
#include "itank.h"
#include "circleofpowereffect.h"

CircleOfPower::CircleOfPower(bool edit_mode):IObject(edit_mode)
{
    _width = 120;

    _height = 120;

    _team = 1;

    _player = 1;

    _tank_activated = false;

    _tileset = getListOfPixmapFromStripImage(QString(":/gt/sprites/circle_of_power_%1.png").arg(_team), _width);

    _graphics_live = getListOfPixmapFromStripImage(QString(":/gt/sprites/live_cp_%1.png").arg(_team), 60);

    _pixmap_effect = getListOfPixmapFromStripImage(QString(":/gt/sprites/regeneration.png"), 60);

    _mini_map_view->setPixmap(QPixmap(QString(":/gt/sprites/mini_map_object_%1.png").arg(_team)));

    _last_tank_cause_live_changed = NULL;

    _tank = NULL;

    _live = 5000;
    _max_live = 10000;
    _tank_direction = 0;
    _index_of_graphics_live = 15;
    _pixmap_tank_direction = QPixmap(QString(":/gt/sprites/tank_direction.png"));
}

void CircleOfPower::setTank(ITank *tank)
{
    _tank = tank;
    _tank->setCircleOfPower(this);
    connect(_tank, &ITank::notifyReborn, this, &CircleOfPower::tankReborn);
}

void CircleOfPower::initObject()
{
    foreach(ITank* tank, _tanks)
        if(tank != _tank)
            tank->addOtherCircleOfPower(this);
}

QPainterPath CircleOfPower::shape() const
{
    QPainterPath path;
    path.addEllipse(-46, -46, 92, 92);
    return path;
}

QRectF CircleOfPower::boundingRect() const
{
    return QRectF(-_width / 2, -_height / 2, _width, _height);
}

void CircleOfPower::save(QDataStream &stream)
{
    IObject::save(stream);

    stream << _team;
    stream << _player;
    stream << _max_live;
    stream << _tank_direction;
}

void CircleOfPower::loadFromData(QDataStream &stream)
{
    IObject::loadFromData(stream);

    stream >> _team;
    stream >> _player;
    stream >> _max_live;
    stream >> _tank_direction;

    _live = _max_live;

    _tileset = getListOfPixmapFromStripImage(QString(":/gt/sprites/circle_of_power_%1.png").arg(_team), _width);

    _graphics_live = getListOfPixmapFromStripImage(QString(":/gt/sprites/live_cp_%1.png").arg(_team), 60);

    _mini_map_view->setPixmap(QPixmap(QString(":/gt/sprites/mini_map_object_%1.png").arg(_team)));
}

QList<QWidget *> CircleOfPower::getControls()
{
    QSpinBox *team = new QSpinBox();
    team->setObjectName("team");
    team->setMaximum(2);
    team->setMinimum(1);
    team->setPrefix("Equipo ");
    team->setValue(_team);
    connect(team,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_team = new Control("Equipo",team);

    QSpinBox *player = new QSpinBox();
    player->setObjectName("player");
    player->setMaximum(3);
    player->setMinimum(1);
    player->setPrefix("Jugador ");
    player->setValue(_player);
    connect(player,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_player = new Control("Jugador",player);

    QSpinBox *live = new QSpinBox();
    live->setObjectName("live");
    live->setMaximum(10000);
    live->setMinimum(1000);
    live->setValue(_max_live);
    connect(live,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_live = new Control("Vida",live);

    QSpinBox *tank_direction = new QSpinBox();
    tank_direction->setObjectName("tank_direction");
    tank_direction->setMaximum(359);
    tank_direction->setMinimum(0);
    tank_direction->setValue(_tank_direction);
    connect(tank_direction,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    Control *c_tank_direction = new Control("Dirección",tank_direction);

    QList<QWidget*> controls = IObject::getControls();
    controls.append(c_team);
    controls.append(c_player);
    controls.append(c_live);
    controls.append(c_tank_direction);

    return controls;
}

int CircleOfPower::live() const
{
    return _live;
}

void CircleOfPower::addDamage(int damage, ITank* tank)
{
    if(_live == 0)
        return;

    _live -= damage;

    if(_live <= 0)
    {
        _live = 0;
        _step = 0;

        _tank->_base_live_regeneration = 0;
        _tank->_base_mana_regeneration = 0;
        _tank->_tank_info._inc_live_regeneration = 0;
        _tank->_tank_info._inc_mana_regeneration = 0;

        tank->addExperience(500);

        emit notifyDesactive(tank);
    }

    if(_live > _max_live)
        _live = _max_live;

    _index_of_graphics_live = _live/_max_live*(_graphics_live.size())-1;

    if(_index_of_graphics_live < 0)
        _index_of_graphics_live = 0;
}

bool CircleOfPower::isEnemy()
{
    return _tank->isEnemy();
}

bool CircleOfPower::isActive()
{
    return _live > 0 && _tank != NULL;
}

void CircleOfPower::setAttribute()
{
    IObject::setAttribute();
    QObject* widget = sender();
    if(widget->objectName() == "team")
    {
        _team = ((QSpinBox*)widget)->value();

        _tileset = getListOfPixmapFromStripImage(QString(":/gt/sprites/circle_of_power_%1.png").arg(_team), _width);
        _mini_map_view->setPixmap(QPixmap(QString(":/gt/sprites/mini_map_object_%1.png").arg(_team)));
    }
    else if(widget->objectName() == "player")
        _player = ((QSpinBox*)widget)->value();
    else if(widget->objectName() == "live")
        _max_live = ((QSpinBox*)widget)->value();
    else if(widget->objectName() == "tank_direction")
        _tank_direction = ((QSpinBox*)widget)->value();
}

void CircleOfPower::gameLoop()
{
    if(_tank != NULL)
    {
        double distance = getDistance(_tank->pos(), pos());

        if(isActive())
        {
            _step++;
            _step = (_step + 1) % _tileset.size();

            if(distance <= 200 && _effect.isEmpty() )
            {
                Effect* effect = new CircleOfPowerEffect(_tank, _tank, 10, 10, 0, 0, false, true, 0, _pixmap_effect, -0.1);
                _tank->addEffect(effect);
                _effect.append(effect);
            }

            update(boundingRect());
        }
        if((distance > 200 && !_effect.isEmpty()) || (!isActive() && !_effect.isEmpty()))
        {
            dynamic_cast<CircleOfPowerEffect*>(_effect.at(0))->setTimeZero();
            _effect.clear();
        }
    }
}

void CircleOfPower::paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    IObject::paintForEditMode(painter, option, widget);

    painter->drawPixmap(-_width / 2, -_height / 2, _width, _height, _tileset.at(_step));

    painter->save();
    painter->rotate(_tank_direction);
    painter->drawPixmap(-11, -16, _pixmap_tank_direction);
    painter->restore();

    painter->drawText(0,0,QString("jugador %1").arg(_player));
}

void CircleOfPower::paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(-_width / 2, -_height / 2, _width, _height, _tileset.at(_step));
    if(isActive())
        painter->drawPixmap(-30, -30, 60, 60, _graphics_live.at(_index_of_graphics_live));
}

void CircleOfPower::tankReborn()
{
    _tank->setPos(pos());
    _tank->setRotation(_tank_direction);
}

int CircleOfPower::player() const
{
    return _player;
}

ITank *CircleOfPower::tank()
{
    return _tank;
}

int CircleOfPower::team() const
{
    return _team;
}
