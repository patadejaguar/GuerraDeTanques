#ifndef TREEAREA_H
#define TREEAREA_H

#include <QList>
#include <QPainterPath>
#include <QPolygon>
#include <QList>

class TreeArea
{

public:
    TreeArea(QPolygon value = QPolygon());

    QPolygon value() const;
    void setValue(const QPolygon &value);

    void addChildren(TreeArea* children);
//    void removeChildren(TreeArea* children);
    QList<QPainterPath> getAreas();

protected:
    QPolygon _value;
    QList<TreeArea*> _childrens;

private:
    void addPath(int height,QList<QPainterPath> &paths);

};

#endif // TREEAREA_H
