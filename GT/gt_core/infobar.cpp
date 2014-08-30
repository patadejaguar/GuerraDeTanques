#include "infobar.h"

ColorNumber::ColorNumber(ColorNumberType type, int value, QGraphicsItem *item, QObject *parent) :
    QObject(parent),
    QGraphicsPathItem(item)
{
    _step = 0;
    _value = value;
    QFont font = QFont("Revue BT",8);
    font.setStyleStrategy(QFont::ForceOutline);
    QPainterPath path;

    if(type == LIVE)
    {
        if(_value < 0)
        {
            path.addText(0,0,font,QString::number(_value));
            setBrush(QColor(COLOR_RED));
            setPen(QPen(QColor(30,0,0)));
        }
        else
        {
            path.addText(0,0,font,"+"+QString::number(_value));
            setBrush(QColor(COLOR_GREEN));
            setPen(QPen(QColor(0,30,0)));
        }
    }
    else if(type == MANA)
    {
        if(_value < 0)
        {
            path.addText(0,0,font,QString::number(_value));
            setBrush(QColor(COLOR_PURPLE));
            setPen(QPen(QColor(20,10,20)));
        }
        else
        {
            path.addText(0,0,font,"+"+QString::number(_value));
            setBrush(QColor(COLOR_BLUE_LIGHT));
            setPen(QPen(QColor(0,15,30)));
        }
    }

    setPath(path);

//    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
//    shadow->setColor(Qt::black);
//    shadow->setOffset(0);
//    shadow->setBlurRadius(5);
//    setGraphicsEffect(shadow);
}

void ColorNumber::advance(int phase)
{
    if(!phase)
        return;
    setPos(pos().x(), pos().y()-0.3);
    setOpacity(double(25-_step) * 0.04);
    if(_step++==25)
    {
        scene()->removeItem(this);
        deleteLater();
    }
}

ColorBar::ColorBar(QColor color, QGraphicsItem *item, int height, double width, QObject *parent):
    QObject(parent),
    QGraphicsItem(item)
{
    _color = color;
    _diference = 0;
    _actual = 100;
    _delay = 0;
    _dash = 0;
    _width = width;
    _height = height;
    _rect = QRectF(0,0,_width,_height);    
}

QRectF ColorBar::boundingRect() const
{
    return QRectF(_rect.x()-1,_rect.y()-1,_rect.width()+1,_rect.height()+1);
}

void ColorBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QLinearGradient gradient(0, 0, 0, _height);
    gradient.setSpread(QGradient::PadSpread);
    gradient.setColorAt(0,QColor(255, 255, 255, 0));
    gradient.setColorAt(0.3,QColor(255, 255, 255,150));
    gradient.setColorAt(0.7,QColor(255, 255, 255, 0));
    painter->setBrush(gradient);    
    painter->drawRect(_rect);

    double actual = _width/100 * _actual;

    if(_actual > 0)
    {
        gradient.setColorAt(0,_color);
        gradient.setColorAt(0.3,_color.lighter(190));
        gradient.setColorAt(0.7,_color);
        painter->setBrush(gradient);
        painter->drawRect(0,0,actual,_height);

    }
    if(_delay > 0)
    {
        double diference = _width/100 * _diference *((double)_delay/10.0);
        painter->setBrush(Qt::white);
        painter->drawRect(actual,0,diference,_height);
    }

    //paint dash lines
    if(_dash > 0)
    {
        painter->setPen(QColor(90,90,90));
        double dash = _width/_dash;
        for(double i = dash; i<_width;i+=dash)
            painter->drawLine(i,1,i,_height-1);
    }
}

void ColorBar::setPercent(int actual, int dash)
{
    _diference = _actual - actual;
    _actual = actual;
    _dash = dash;
    _delay = 10;
}

void ColorBar::advance(int phase)
{
    if(!phase)
        return;

    if(_delay>0)
        _delay--;
    else
        _delay = 0;
    update(boundingRect());
}

InfoBar::InfoBar(QString nick, int rank, ColorTeam color_team)
    :QGraphicsItemGroup(), QObject()
{
    _nick = new QGraphicsTextItem(nick,this);
    _nick->setFont(QFont("Revue BT",8));

    _rank = new QGraphicsPixmapItem(QPixmap(":/gt/ranks/rank_"+QString::number(rank)+".png"),this);
    _rank->setScale(0.8);
    _pointer = new QGraphicsPixmapItem(QPixmap(":/gt/pointers/pointer_"+QString::number(color_team)+".png"),this);

    _live_bar = new ColorBar(Qt::green,this);
    _mana_bar = new ColorBar(QColor(0,162,232),this);

    _rank->setTransformationMode(Qt::SmoothTransformation);
    _pointer->setTransformationMode(Qt::SmoothTransformation);

    _rank->setZValue(100);
    _pointer->setZValue(101);
    _live_bar->setZValue(102);
    _mana_bar->setZValue(103);
    _nick->setZValue(104);

//    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
//    shadow->setColor(Qt::black);
//    shadow->setOffset(0);
//    shadow->setBlurRadius(5);
//    setGraphicsEffect(shadow);

    _rank->setPos(-50,-50);
    _pointer->setPos(-24,-54);
    _live_bar->setPos(-24,-41);
    _mana_bar->setPos(-24,-35);
    _nick->setPos(-18,-60);
}

InfoBar::~InfoBar()
{
    delete _rank;
    delete _pointer;
    delete _live_bar;
    delete _mana_bar;
    delete _nick;
}

void InfoBar::setLiveBarData(int current, int max)
{
    _live_bar->setPercent(100 * current / max, max/100);
}

void InfoBar::setManaBarData(int current, int max)
{
    _mana_bar->setPercent(100 * current / max, max/100);
}

void InfoBar::setPos(QPointF pos)
{
    QGraphicsItemGroup::setPos(pos);
}
