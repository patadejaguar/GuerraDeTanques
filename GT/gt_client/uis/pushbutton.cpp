#include "pushbutton.h"

PushButton::PushButton(QString base_pixmap, QWidget *parent) :
    QPushButton(parent)
{
    _base_pixmap = base_pixmap;
    _pixmap = ":/button/"+_base_pixmap+"_release.png";
    _press = false;
    setMouseTracking(true);
}

void PushButton::resetPixmap()
{
    _pixmap = ":/button/"+_base_pixmap+"_release.png";
}

void PushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(_pixmap));
    painter.end();
}

void PushButton::leaveEvent(QEvent *event)
{
    _pixmap = ":/button/"+_base_pixmap+"_release.png";
    QPushButton::leaveEvent(event);
}

void PushButton::mouseMoveEvent(QMouseEvent *event)
{
    _press ? _pixmap = ":/button/"+_base_pixmap+"_press.png" : _pixmap = ":/button/"+_base_pixmap+"_hover.png";
    QPushButton::mouseMoveEvent(event);
}

void PushButton::mousePressEvent(QMouseEvent *event)
{
    _pixmap = ":/button/"+_base_pixmap+"_press.png";
    _press = true;
    QPushButton::mousePressEvent(event);
}

void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    _pixmap = ":/button/"+_base_pixmap+"_release.png";
    _press = false;
    QPushButton::mouseReleaseEvent(event);
}
