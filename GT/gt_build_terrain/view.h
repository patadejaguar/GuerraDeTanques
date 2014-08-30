#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QWheelEvent>

#include "scene.h"

class View : public QGraphicsView
{
        Q_OBJECT
    public:
        explicit View(QWidget *parent = 0);

    signals:
        void zoomChange(int value);

    public slots:
        void setZoom(int value);

    protected:
        void wheelEvent(QWheelEvent *event);
        void leaveEvent(QEvent *);
        void enterEvent(QEvent *);
        void keyPressEvent(QKeyEvent* event);
        void keyReleaseEvent(QKeyEvent* event);

    private:
        int _scale;
        bool _can_scale;
};

#endif // VIEW_H
