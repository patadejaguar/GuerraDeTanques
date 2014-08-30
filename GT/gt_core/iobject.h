#ifndef IOBJECT_H
#define IOBJECT_H

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include "gt_core_global.h"
#include "effect.h"
#include "utilities.h"
#include "configuration.h"

class ITank;

class GT_CORESHARED_EXPORT Control: public QWidget
{
    public:
        Control(QString name, QWidget* modifier);
};


class GT_CORESHARED_EXPORT IObject: public QGraphicsObject
{

        Q_OBJECT

    public:

        IObject(bool edit_mode = false);
        virtual ~IObject();

        virtual inline void     initObject(){}

        virtual QList<QWidget*> getControls();

        virtual QPainterPath    shape() const = 0;

        virtual QRectF          boundingRect() const = 0;

        virtual QString         nameForDependency() const = 0;

        virtual void            save(QDataStream &stream);

        virtual void            loadFromData(QDataStream &stream);

        enum                    { Type = UserType + 1 };
        int                     type() const{return Type;}

        void                    setPos(const QPointF &pos);

        void                    setPos(qreal x, qreal y);

        QGraphicsPixmapItem*    getMiniMapView() const;

        void                    setTanks(QList<ITank*> tanks);

        void                    advance(int phase);

    public slots:

        virtual void            setAttribute();

    signals:

        void                    updateXPos(int);

        void                    updateYPos(int);

    protected:

        virtual void            gameLoop() = 0;

        void                    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        virtual void            paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        virtual void            paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

        //atributes
        QList<QPixmap>          _tileset;

        int                     _width;

        int                     _height;

        int                     _step;

        QList<ITank*>           _tanks;

        QGraphicsPixmapItem*    _mini_map_view;

    private:

        bool                    _edit_mode;

};

class GT_CORESHARED_EXPORT IObjectFactory
{
    public:

        virtual                 ~IObjectFactory(){}

        virtual QPixmap         getPreview() = 0;

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        virtual QString         pluginName()     const = 0;
        virtual QString         nameForDependency() const = 0;
        virtual int             gameVersion()  const = 0;

        virtual QString         description() const = 0;

        //crea una instancia del objeto
        virtual IObject*        createInstance(bool edit_mode = false) = 0;

    protected:
        QPixmap _preview;
};

Q_DECLARE_INTERFACE(IObjectFactory, "xavier.gt.iobjectfactory")

#endif // IOBJECT_H
