#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QMetaProperty>

#include "drawpoint.h"
#include "iobject.h"
//#include "polygon.h"

//enum ToolObst {TOOL_RECTANGLE, TOOL_POLYGON};

class Scene : public QGraphicsScene
{
        Q_OBJECT

    public:
        explicit Scene(QObject *parent = 0);
        void setSnapToGrid(bool value);
        void setActivateCursor(bool value);
        bool isActivateCursor();
        void setVisiblePreview(bool value);
        void updateObjectPreview();

//        ToolObst tool_obstruction() const;
//        void setTool_obstruction(const ToolObst &tool_obstruction);

    signals:
        void updateObjectProperties(QList<QWidget*> controls);
        void noObjectSelected();

    public slots:
        void clear();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);

    private slots:
        void refreshSelectedItem();

    private:
        void refreshLastDrawPoints(QPointF pos);
        QList<DrawPoint*>_last_draw_points;

        bool _press;
        QGraphicsPixmapItem *_object_preview;
        IObject *_selected_object;

        bool _snap_to_grid;
        bool _activate_cursor;

//        ToolObst       _tool_obstruction;
//        QList<Point*> _polygon_points;
//        Polygon p;
};

#endif // SCENE_H
