#include "treearea.h"

TreeArea::TreeArea(QPolygon value)
{
    _value = value;
}

QPolygon TreeArea::value() const
{
    return _value;
}

void TreeArea::setValue(const QPolygon &value)
{
    _value = value;
}

void TreeArea::addChildren(TreeArea *children)
{
    bool contains_in_childrens = false;
    for(int i = 0; i < _childrens.size() && !contains_in_childrens; ++i)
    {
        if(_childrens.at(i)->value().containsPoint(children->value().at(0),Qt::WindingFill))
        {
            _childrens.at(i)->addChildren(children);
            contains_in_childrens = true;
        }
    }
    if(!contains_in_childrens && _value.containsPoint(children->value().at(0),Qt::WindingFill))
        _childrens.append(children);
}

QList<QPainterPath> TreeArea::getAreas()
{
    QList<QPainterPath> areas;
    addPath(0,areas);
    return areas;
}

void TreeArea::addPath(int height, QList<QPainterPath> &paths)
{
    if(height%2 == 0)
    {
        QPainterPath area;
        area.addPolygon(_value);
        foreach (TreeArea* children, _childrens)
            area.addPolygon(children->value());
        paths.append(area);
    }
    foreach (TreeArea* children, _childrens)
        children->addPath(height+1,paths);
}

//void TreeArea::removeChildren(TreeArea *children)
//{
//    _childrens.removeAll(children);
//}

