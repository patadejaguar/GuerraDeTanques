#ifndef FADELABELEFFECT_H
#define FADELABELEFFECT_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>

class FadeLabelEffect : public QLabel
{
        Q_OBJECT
        Q_PROPERTY(int _opacity READ opacity WRITE setOpacity NOTIFY notifyOpacity)

    public:
        explicit FadeLabelEffect(QString text, QList<FadeLabelEffect*>* list, QWidget *parent = 0);
        ~FadeLabelEffect();

        int opacity() const;
        void setOpacity(int opacity);

    signals:

        void    notifyOpacity(int);

    public slots:

    private:

        int                         _opacity;
        QPropertyAnimation*         _animation;
        QList<FadeLabelEffect*>*    _list;

};

#endif // FADELABELEFFECT_H
