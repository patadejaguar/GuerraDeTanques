#ifndef OBJECT_H
#define OBJECT_H

#include <QtGui>

#include "iobject.h"
#include "soundengine.h"

#define gt_plugin "xavier_object_tablet_of_experience_v1";

class Object: public IObject
{
        Q_OBJECT

    public:

        Object(bool edit_mode = false);

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

        int                     _experience;

        int                     _cooldown;

        int                     _current_cooldown;

        int                     _one_second;

        QList<QPixmap>          _explode;

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
            QPixmap pixmap(":/xavier_object_tablet_of_experience/tablet_of_experience.png");
            _preview = pixmap.copy(0,0,pixmap.height(),pixmap.height());
        }
        QPixmap getPreview(){return _preview;}

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        inline QString          pluginName() const          {return "Tableta de experiencia";}
        inline QString          nameForDependency() const   {return gt_plugin;}
        inline int              gameVersion() const         {return 1;}

        inline QString          description() const         {return "Aumenta la experiencia del tanque.";}

        //crea una instancia del objeto
        inline virtual IObject*         createInstance(bool edit_mode = false){return new Object(edit_mode);}
};
#endif // OBJECT_H
