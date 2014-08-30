#ifndef OBJECT_H
#define OBJECT_H

#include <QtGui>

#include "iobject.h"
#include "soundengine.h"

class ITank;

class Object: public IObject
{
        Q_OBJECT

    public:

        Object(QList<QPixmap> tile_set, int time, ITank* tank, bool edit_mode = false);

        ~Object();

        QPainterPath            shape() const;

        QRectF                  boundingRect() const;

        inline QString          nameForDependency() const{return "viuda_negra_trampa";}

    protected:

        void                    gameLoop();

        void                    paintForEditMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

        void                    paintForExecuteMode(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    private:

        int                     _time_effect;

        int                     _time_live;

        bool                    _activated;

        QList<QPixmap>          _tileset;

        SoundEngine*            _sound;

        ITank*                  _tank;
};

class ObjectFactory: public QObject, public IObjectFactory
{
        Q_OBJECT
        Q_INTERFACES(IObjectFactory)

    public:
        ObjectFactory()
        {
            _tileset = getListOfPixmapFromStripImage(":/xavier_tank_viuda_negra/sprites/skill_2.png", 100);
            _preview = _tileset.at(0);
        }
        QPixmap getPreview(){return _preview;}

        //funciones para obtener el nombre, la versión del cliente para la cual se desarrolló el plugin y la versión del plugin
        inline QString          pluginName() const          {return "Trampa de araña";}
        inline QString          nameForDependency() const   {return "viuda_negra_trampa";}
        inline int              gameVersion() const         {return 1;}

        inline QString          description() const         {return "Ralentiza al que se caiga en ella.";}

        //crea una instancia del objeto
        inline IObject*         createInstance(bool edit_mode){Q_UNUSED(edit_mode);return NULL;}
        inline IObject*         createInstance(int time, ITank* tank){return new Object(_tileset, time, tank);}

    private:
        QList<QPixmap>          _tileset;

};
#endif // OBJECT_H
