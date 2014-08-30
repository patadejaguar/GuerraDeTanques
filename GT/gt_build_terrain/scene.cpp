#include "scene.h"
#include "terrain.h"
#include <typeinfo>

#include <QDebug>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    _press = false;

    _object_preview = new QGraphicsPixmapItem();
    _object_preview->setZValue(1000);
    _object_preview->setOpacity(0.8);
    _object_preview->setVisible(false);
    addItem(_object_preview);

    _snap_to_grid = false;
    _activate_cursor = false;

    connect(this,SIGNAL(selectionChanged()),this,SLOT(refreshSelectedItem()));
}

void Scene::setVisiblePreview(bool value)
{
    Terrain *terrain = Terrain::instance();
    if(terrain->currentLayer() == L_TERRAIN)
    {
        foreach (DrawPoint * draw_point, _last_draw_points)
            draw_point->setVisible(value);

        foreach (IObject* obj, Terrain::instance()->objects())
            obj->setFlag(QGraphicsItem::ItemIsSelectable,false);
    }
    else if(terrain->currentLayer() == L_OBJECT)
    {
        _object_preview->setVisible(value && !_activate_cursor);
        foreach (IObject* obj, Terrain::instance()->objects())
            obj->setFlag(QGraphicsItem::ItemIsSelectable,_activate_cursor);
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(Terrain::instance()->currentLayer() == L_TERRAIN)
    {
        foreach (DrawPoint * last_draw_point, _last_draw_points)
            last_draw_point->setStatus(PRESS);
    }
    else if(Terrain::instance()->currentLayer() == L_OBJECT)
    {

    }
    _press = true;
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _press = false;
    if(Terrain::instance()->currentLayer() == L_TERRAIN)
    {
        foreach (DrawPoint * last_draw_point, _last_draw_points)
            last_draw_point->setStatus(HOVER);
    }
    else if(Terrain::instance()->currentLayer() == L_OBJECT)
    {

        if(!_activate_cursor)
        {
            IObject* object = Terrain::instance()->activateObjectFactory()->createInstance(true);
            object->setFlag(QGraphicsItem::ItemIsSelectable);
            QPointF pos = event->scenePos();
            if(_snap_to_grid)
            {
                int w = Terrain::instance()->tileWidth();
                int h = Terrain::instance()->tileHeight();
                int inc_x = 0;
                int inc_y = 0;
                if(int(pos.x())%w > w/2)
                    inc_x = w;
                if(int(pos.y())%h > h/2)
                    inc_y = h;
                pos.setX(int(pos.x())/w*w+inc_x);
                pos.setY(int(pos.y())/h*h+inc_y);
            }
            object->setPos(pos);

            Terrain::instance()->addObject(object);
            addItem(object);
        }
    }
//    else if(Terrain::instance()->currentLayer() == L_OBSTRUCTIONS)
//    {
//        if(!_activate_cursor)
//        {
//            if(_tool_obstruction == TOOL_POLYGON)
//            {
//                QGraphicsItem* item = itemAt(event->scenePos(), views().at(0)->transform());
//                Point* point = dynamic_cast<Point*> (item);
//                if(point && _polygon_points.size() >= 3)
//                {
////                    Terrain::instance()->polygons().append(new Polygon(_polygon_points));
//                    _polygon_points.clear();
//                }
//                else
//                {
//                    _polygon_points.append(new Point(event->scenePos().x(), event->scenePos().y()));
//                }
//            }
//        }
//    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(Terrain::instance()->currentLayer() == L_TERRAIN)
    {
        foreach (DrawPoint * last_draw_point, _last_draw_points)
            last_draw_point->setStatus(RELEASE);
        _last_draw_points.clear();

        QPointF pos = event->scenePos();

        QGraphicsItem* item = itemAt(pos, views().at(0)->transform());
        DrawPoint *draw_point = dynamic_cast<DrawPoint*>(item);
        if(draw_point != NULL)
            _last_draw_points.append(draw_point);

        refreshLastDrawPoints(pos);

        if(_press)
            foreach (DrawPoint * last_draw_point, _last_draw_points)
                last_draw_point->setStatus(PRESS);
        else
            foreach (DrawPoint * last_draw_point, _last_draw_points)
                last_draw_point->setStatus(HOVER);
    }
    else if(Terrain::instance()->currentLayer() == L_OBJECT)
    {
        if(_activate_cursor)
        {
            if(_press && selectedItems().size()>0)
            {
                QPointF pos = event->scenePos();
                if(_snap_to_grid)
                {
                    int w = Terrain::instance()->tileWidth();
                    int h = Terrain::instance()->tileHeight();
                    int inc_x = 0;
                    int inc_y = 0;
                    if(int(pos.x())%w > w/2)
                        inc_x = w;
                    if(int(pos.y())%h > h/2)
                        inc_y = h;
                    pos.setX(int(pos.x())/w*w+inc_x);
                    pos.setY(int(pos.y())/h*h+inc_y);
                }
                IObject *obj = dynamic_cast<IObject*>(selectedItems().at(0));
                if(obj)
                    obj->setPos(pos);
            }
        }
        else
        {            
            QPointF pos = event->scenePos();
            if(_snap_to_grid)
            {
                int w = Terrain::instance()->tileWidth();
                int h = Terrain::instance()->tileHeight();
                int inc_x = 0;
                int inc_y = 0;
                if(int(pos.x())%w > w/2)
                    inc_x = w;
                if(int(pos.y())%h > h/2)
                    inc_y = h;
                pos.setX(int(pos.x())/w*w+inc_x);
                pos.setY(int(pos.y())/h*h+inc_y);
            }
            _object_preview->setPos(pos.x()-_object_preview->pixmap().width()/2,pos.y()-_object_preview->pixmap().height()/2);
        }
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        if(selectedItems().size() > 0)
        {
            IObject * obj = dynamic_cast<IObject*>(selectedItems().at(0));
            Terrain::instance()->removeObject(obj);
            removeItem(obj);
            delete obj;
            emit noObjectSelected();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::clear()
{
    _last_draw_points.clear();
    _press = false;
    QGraphicsScene::clear();

    _object_preview = new QGraphicsPixmapItem();
    _object_preview->setZValue(1000);
    _object_preview->setOpacity(0.8);
    _object_preview->setVisible(false);
    addItem(_object_preview);
}

void Scene::setSnapToGrid(bool value)
{
    _snap_to_grid = value;
}

void Scene::setActivateCursor(bool value)
{
    _activate_cursor = value;
}

bool Scene::isActivateCursor()
{
    return _activate_cursor;
}

void Scene::updateObjectPreview()
{
    QPixmap preview = Terrain::instance()->activateObjectFactory()->getPreview();
    _object_preview->setPixmap(preview);
}

void Scene::refreshLastDrawPoints(QPointF pos)
{
    int cursor = Terrain::instance()->cursorSize();
    int tile_width = Terrain::instance()->tileWidth();
    int tile_height = Terrain::instance()->tileHeight();

    for(int i = 0; i < cursor; ++i )
        for(int j= 0; j < cursor-i; ++j)
            if(!(i==0 && j==0))
            {
                QPointF pos_1 = QPointF(pos.x()+tile_width*i,pos.y()+tile_height*j);
                QGraphicsItem* item_1 = itemAt(pos_1,views().at(0)->transform());
                DrawPoint *draw_point_1 = dynamic_cast<DrawPoint*>(item_1);
                if(draw_point_1 != NULL)
                    _last_draw_points.append(draw_point_1);

                QPointF pos_2 = QPointF(pos.x()-tile_width*i,pos.y()-tile_height*j);
                QGraphicsItem* item_2 = itemAt(pos_2,views().at(0)->transform());
                DrawPoint *draw_point_2 = dynamic_cast<DrawPoint*>(item_2);
                if(draw_point_2 != NULL)
                    _last_draw_points.append(draw_point_2);

                QPointF pos_3 = QPointF(pos.x()+tile_width*i,pos.y()-tile_height*j);
                QGraphicsItem* item_3 = itemAt(pos_3,views().at(0)->transform());
                DrawPoint *draw_point_3 = dynamic_cast<DrawPoint*>(item_3);
                if(draw_point_3 != NULL)
                    _last_draw_points.append(draw_point_3);

                QPointF pos_4 = QPointF(pos.x()-tile_width*i,pos.y()+tile_height*j);
                QGraphicsItem* item_4 = itemAt(pos_4,views().at(0)->transform());
                DrawPoint *draw_point_4 = dynamic_cast<DrawPoint*>(item_4);
                if(draw_point_4 != NULL)
                    _last_draw_points.append(draw_point_4);
            }
}

//ToolObst Scene::tool_obstruction() const
//{
//    return _tool_obstruction;
//}

//void Scene::setTool_obstruction(const ToolObst &tool_obstruction)
//{
//    _tool_obstruction = tool_obstruction;
//}

void Scene::refreshSelectedItem()
{
    if(_activate_cursor && selectedItems().size()>0)
    {
        IObject *obj = dynamic_cast<IObject*>(selectedItems().at(0));
        if(obj)
            emit updateObjectProperties(obj->getControls());
        else
            emit noObjectSelected();
    }
    else
        emit noObjectSelected();
}
