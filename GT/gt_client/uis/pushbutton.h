#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QFocusEvent>
#include <Qpainter>

class PushButton : public QPushButton
{
        Q_OBJECT
    public:

        explicit    PushButton(QString base_pixmap, QWidget *parent = 0);

        void        resetPixmap();

    signals:

    public slots:

    protected:

        void        paintEvent(QPaintEvent *event);

        void        leaveEvent(QEvent *event);

        void        mouseMoveEvent(QMouseEvent *event);

        void        mousePressEvent(QMouseEvent *event);

        void        mouseReleaseEvent(QMouseEvent *event);

    private:

        QString     _base_pixmap;

        QString     _pixmap;

        bool        _press;
};

#endif // PUSHBUTTON_H
