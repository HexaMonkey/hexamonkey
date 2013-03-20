#include "treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : QObject(),
      parentItem(parent),
      itemData(data)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

void TreeItem::removeChildren()
{
    emit childrenRemoved();
    qDeleteAll(childItems);
    childItems.clear();
}

TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}

int TreeItem::childCount() const
{
    return childItems.count();
}

int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

bool TreeItem::synchronised()
{
    return true;
}

