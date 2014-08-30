#include "filterproxymodel.h"
#include <QDateTime>

FilterProxyModel::FilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool FilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QDirModel *model = (QDirModel *)sourceModel();
    QModelIndex index = model->index(source_row, 0, source_parent);

    if(model->isDir(index))
        return true;

    if( _filter_list.count() == 0 )
        return true;

    QString filter;
    foreach(filter, _filter_list)
        if( model->data(index).toString().right(filter.length()).toLower() == filter )
            return true;

    return false;
}

bool FilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
 {
     QString left_data = sourceModel()->data(left).toString();
     QString right_data = sourceModel()->data(right).toString();

     return left_data > right_data;

 }
