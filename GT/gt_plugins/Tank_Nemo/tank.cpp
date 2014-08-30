#include "tank.h"

Tank::Tank(TankInfo tank_info, QString nick, int rank, ColorTeam color_team)
    :ITank(tank_info, nick, rank, color_team)
{
    //atributos para las animaciones
    _frame_animation_whell_1 = 0;
    _frame_animation_whell_2 = 0;

    //crea las piezas del tanque !!!
    _animation_whell = getListOfPixmapFromStripImage(":/xavier_tank_nemo/sprites/whell.png",18);
    _body = QPixmap(":/xavier_tank_nemo/sprites/body.png");
    _turret = QPixmap(":/xavier_tank_nemo/sprites/turret.png");

   //posiciones relativas de los objetos que componen el tanque para que queden en el centro
    _body_position = QPoint(-16,-19);
    _turret_position = QPoint(-11,-27);
    _whell_1_position = QPoint(-24,-20);
    _whell_2_position = QPoint(6,-20);

    //para detección de colisiones
    _rect_body = QRect(0, 0, 46, 38);
    _shape_body.addRect(_rect_body);
}

Tank::~Tank()
{

}

void Tank::reborn()
{
    ITank::reborn();
    _frame_animation_whell_1 = 0;
    _frame_animation_whell_2 = 0;
}

QPainterPath Tank::shape() const
{
    QPainterPath shape;
    QTransform transformation_turret;
    transformation_turret.translate(0,-3);
    transformation_turret.rotate(_turret_rotation);
    transformation_turret.translate(-4,-24);

    shape.addPath(_shape_body.translated(_body_position.x()-7, _body_position.y()));

    return shape;
}

void Tank::paintForEnemies(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintForFriends(painter,option,widget);
}

void Tank::paintForFriends(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(_whell_1_position,_animation_whell.at(_frame_animation_whell_1));
    painter->drawPixmap(_whell_2_position,_animation_whell.at(_frame_animation_whell_2));
    painter->drawPixmap(_body_position,_body);
    painter->translate(0,-3);
    painter->rotate(_turret_rotation-rotation());
    painter->translate(-11,-24);
    painter->drawPixmap(0,0,_turret);
}

void Tank::createSkills()
{
    _skill_1 = new Skill1Factory(this, tankInfo()._skill_info_1);
    _skill_2 = new Skill2Factory(this, tankInfo()._skill_info_2);
    _skill_3 = new Skill3Factory(this, tankInfo()._skill_info_3);
}

QPoint Tank::getTurretPosition()
{
    return _turret_position;
}

void Tank::gameLoop()
{
}

void Tank::move()
{
    ITank::move();

    int roll = 0;

    if(_move_forward)
        ++roll;
    if(_move_backward)
        --roll;

    _frame_animation_whell_1 += roll;
    if(_frame_animation_whell_1 > 9)
        _frame_animation_whell_1 = 0;
    else if(_frame_animation_whell_1 < 0)
        _frame_animation_whell_1 = 9;

    _frame_animation_whell_2 += roll;
    if(_frame_animation_whell_2 > 9)
        _frame_animation_whell_2 = 0;
    else if(_frame_animation_whell_2 < 0)
        _frame_animation_whell_2 = 9;
}

void Tank::rotate()
{
    ITank::rotate();

    int roll = 0;

    if(_rotate_left)
        ++roll;
    if(_rotate_right)
        --roll;

    _frame_animation_whell_1 -= roll;
    if(_frame_animation_whell_1 > 9)
        _frame_animation_whell_1 = 0;
    else if(_frame_animation_whell_1 < 0)
        _frame_animation_whell_1 = 9;

    _frame_animation_whell_2 += roll;
    if(_frame_animation_whell_2 > 9)
        _frame_animation_whell_2 = 0;
    else if(_frame_animation_whell_2 < 0)
        _frame_animation_whell_2 = 9;
}

