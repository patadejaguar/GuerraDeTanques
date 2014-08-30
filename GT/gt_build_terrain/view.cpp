#include "view.h"

View::View(QWidget *parent) :
    QGraphicsView(parent)
{
    _scale = 100;
    _can_scale = false;
    setRenderHints(QPainter::Antialiasing/*|QPainter::SmoothPixmapTransform*/);
//    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void View::wheelEvent(QWheelEvent *event)
{
    if(_can_scale)
    {
        const int degrees = event->delta() / 8;
        int steps = degrees / 15;

        int scaleFactor = 10;
        int minFactor = 10;
        int maxFactor = 500;

        int value = _scale;

        if(steps > 0)
            value = (value >= maxFactor) ? maxFactor : (value + scaleFactor);
        else
            value = (value <= minFactor) ? minFactor : (value - scaleFactor);

        setZoom(value);
    }
    else
        QGraphicsView::wheelEvent(event);
}

void View::leaveEvent(QEvent *)
{
    (dynamic_cast<Scene*>(scene()))->setVisiblePreview(false);
}

void View::enterEvent(QEvent *)
{
    (dynamic_cast<Scene*>(scene()))->setVisiblePreview(true);
}

void View::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Control)
        _can_scale = true;
    else
        QGraphicsView::keyPressEvent(event);
}

void View::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
        _can_scale = false;
    else
        QGraphicsView::keyReleaseEvent(event);
}

void View::setZoom(int value)
{
    if(_scale == value)
        return;

    _scale = value;
    QTransform transform;
    setTransform(transform.scale((double)_scale/100,(double)_scale/100));
    emit zoomChange(value);
}
