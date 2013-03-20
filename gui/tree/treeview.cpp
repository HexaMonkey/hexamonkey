#include "treeview.h"

TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);
    if(current.isValid())
    {
        QModelIndex realCurrent = current.sibling(current.row(), 0);
        emit selected(realCurrent);
    }
}
