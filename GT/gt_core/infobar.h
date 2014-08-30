#ifndef INFOBAR_H
#define INFOBAR_H

#include <QGraphicsItemGroup>
//#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QLinearGradient>
#include <QPainter>
#include <QColor>
#include <QGraphicsPathItem>
#include <QFont>
#include <QPen>
#include <QBrush>

#include "gt_core_global.h"
#include "utilities.h"

class GT_CORESHARED_EXPORT ColorNumber : public QObject, public QGraphicsPathItem
{
        Q_OBJECT

    public:

        explicit                    ColorNumber(ColorNumberType type, int value, QGraphicsItem *item = 0, QObject *parent = 0);

    protected:

        void                        advance(int phase);

    private:

        int                         _value;

        int                         _step;
};

class GT_CORESHARED_EXPORT ColorBar : public QObject, public QGraphicsItem
{
    public:

        explicit                    ColorBar(QColor color, QGraphicsItem *item = 0, int height = 6, double width = 60, QObject *parent = 0);

        void                        setPercent(int actual, int dash);

    protected:

        QRectF                      boundingRect() const;

        void                        paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                        advance(int phase);

    private:

        QRectF                      _rect;

        QColor                      _color;

        int                         _diference;

        int                         _delay;

        int                         _actual;

        double                      _width;

        int                         _height;

        double                      _dash;
};

class GT_CORESHARED_EXPORT InfoBar: public QGraphicsItemGroup, public QObject
{

    public:

        explicit                    InfoBar(QString nick, int rank, ColorTeam color_team);

        ~InfoBar();

        void                        setLiveBarData(int current, int max);

        void                        setManaBarData(int current, int max);

        void                        setPos(QPointF pos);

    private:

        QGraphicsPixmapItem*        _rank;

        QGraphicsPixmapItem*        _pointer;

        ColorBar*                   _live_bar;

        ColorBar*                   _mana_bar;

        QGraphicsTextItem*          _nick;
};

#endif // INFOBAR_H
