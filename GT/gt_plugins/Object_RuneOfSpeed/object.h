#ifndef OBJECT_H
#define OBJECT_H

#include <QtGui>

#include "iobject.h"
#include "soundengine.h"

#define gt_plugin "xavier_object_rune_of_speed_v1";

class Object: public IObject
{
        Q_OBJECT

    public:

        Object(QList<QPixmap> tile_set, QList<QPixmap> animation, bool edit_mode = false);

        ~Object();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

        inline QString          nameForDependency() const{return gt_plugin;}

        virtual void            save(QDataStream &stream);

        virtual void            loadFromData(QDataStream &stream);

        QList<QWidget*>         getControls();

    public slots:

        void                    setAttribute();

    protected:

        void                    gameLoop();

        void                    paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:

        int                     _bonus;

        int                     _time;

        int                     _cooldown;

        int                     _current_cooldown;

        int                     _one_second;

        QList<QPixmap>          _animation;

        QList<QPixmap>          _effect_animation;

        int                     _frame_explode;

        SoundEngine*            _sound;
};

class ObjectFactory: public QObject, public IObjectFactory
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "xavier.gt.iobjectfactory")
        Q_INTERFACES(IObjectFactory)

    public:
        ObjectFactory()
        {
            QPixmap pixmap(":/xavier_object_rune_of_speed/rune_of_speed.png");
            _preview = pixmap.copy(0,0,pixmap.height(),pixmap.height());

            _tileset = getListOfPixmapFromStripImage(":/xavier_object_rune_of_speed/rune_of_speed.png",60);
            _explode = getListOfPixmapFromStripImage(":/xavier_object_rune_of_speed/explode.png",60);
        }
        QPixmap getPreview(){return _preview;}

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        inline QString          pluginName() const          {return "Runa de velocidad";}
        inline QString          nameForDependency() const   {return gt_plugin;}
        inline int              gameVersion() const         {return 1;}

        inline QString          description() const         {return "Aumenta la velocidad del tanque por un período de tiempo.";}

        //crea una instancia del objeto
        inline virtual IObject*         createInstance(bool edit_mode = false){return new Object(_tileset, _explode, edit_mode);}

    private:
        QList<QPixmap>          _tileset;
        QList<QPixmap>          _explode;

};
#endif // OBJECT_H
