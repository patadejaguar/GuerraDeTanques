#include "iobject.h"
#include "itank.h"

Control::Control(QString name, QWidget *modifier):
    QWidget()
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(new QLabel(name));
    layout->addWidget(modifier);
}

IObject::IObject(bool edit_mode):
    QGraphicsObject()
{
    _edit_mode = edit_mode;

    _step = 0;

    _mini_map_view = new QGraphicsPixmapItem(QPixmap(":/gt/sprites/mini_map_object.png"));

    _mini_map_view->setZValue(10);
}

IObject::~IObject()
{

}

QList<QWidget *> IObject::getControls()
{
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    QSpinBox *pos_x = new QSpinBox();
    pos_x->setObjectName("pos_x");
    pos_x->setMaximum(65535);
    pos_x->setValue(pos().x());
    connect(pos_x,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    connect(this,SIGNAL(updateXPos(int)),pos_x,SLOT(setValue(int)));
    Control *c_pos_x = new Control("X",pos_x);

    QSpinBox *pos_y = new QSpinBox();
    pos_y->setObjectName("pos_y");
    pos_y->setMaximum(65535);
    pos_y->setValue(pos().y());
    connect(pos_y,SIGNAL(valueChanged(int)),this,SLOT(setAttribute()));
    connect(this,SIGNAL(updateYPos(int)),pos_y,SLOT(setValue(int)));
    Control *c_pos_y = new Control("Y",pos_y);

    QList<QWidget*> controls;

    layout->addWidget(c_pos_x);
    layout->addWidget(c_pos_y);
    controls.append(widget);
    //        controls.append(c_pos_x);
    //        controls.append(c_pos_y);

    return controls;
}

void IObject::save(QDataStream &stream)
{
    stream << nameForDependency();
    stream << x();
    stream << y();
}

void IObject::loadFromData(QDataStream &stream)
{
    qreal x, y;
    stream >> x;
    stream >> y;
    setPos(x,y);
}

void IObject::setPos(const QPointF &pos)
{
    QGraphicsObject::setPos(pos);

    if(_mini_map_view->scene() && scene())
    {
        qreal x_p = QGraphicsObject::pos().x()/scene()->width()*100;
        qreal y_p = QGraphicsObject::pos().y()/scene()->height()*100;
        qreal x = _mini_map_view->scene()->width()*x_p/100;
        qreal y = _mini_map_view->scene()->height()*y_p/100;
        _mini_map_view->setPos(x-_mini_map_view->boundingRect().width()/2,y-_mini_map_view->boundingRect().height()/2);
    }

    emit updateXPos(pos.x());
    emit updateYPos(pos.y());

}

void IObject::setPos(qreal x, qreal y)
{
    setPos(QPointF(x,y));
}

QGraphicsPixmapItem *IObject::getMiniMapView() const
{
    return _mini_map_view;
}

void IObject::setTanks(QList<ITank*> tanks)
{
    _tanks = tanks;
}

void IObject::advance(int phase)
{
    if(!phase)
        return;

    gameLoop();
}

void IObject::setAttribute()
{
    QObject* widget = sender();
    if(widget->objectName() == "pos_x")
        setPos(((QSpinBox*)widget)->value(), pos().y());
    else if(widget->objectName() == "pos_y")
        setPos(pos().x(),((QSpinBox*)widget)->value());
}

void IObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(_edit_mode)
    {
        const qreal zoom = option->levelOfDetailFromTransform(painter->worldTransform());
        if(zoom <= 0.4)
        {
            QTransform transform = painter->worldTransform();
            qreal inv= 1.0/zoom;
            transform.scale(inv, inv);
            painter->setTransform(transform);
            painter->drawPixmap(-_mini_map_view->pixmap().width()/2, -_mini_map_view->pixmap().height()/2, _mini_map_view->pixmap());
        }
        else
            paintForEditMode(painter,option,widget);
    }
    else
        paintForExecuteMode(painter,option,widget);
}

void IObject::paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(isSelected())
    {
        painter->save();
        QPen pen(Qt::DashLine);
        pen.setColor(QColor(0,122,174));
        painter->setPen(pen);
        painter->setBrush(QBrush(QColor(0,162,232,40)));
        painter->drawRect(boundingRect());
        painter->restore();
    }
}
