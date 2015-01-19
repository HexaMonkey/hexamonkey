//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "gui/tree/treeitem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : TreeItem(data, parent, parent)
{
}

TreeItem *TreeItem::RootItem(const QList<QVariant> &data, QObject *owner)
{
    return new TreeItem(data, nullptr, owner);
}

void TreeItem::appendChild(TreeItem *item)
{
    _childItems.append(item);
}

bool TreeItem::removeChildren()
{
    onChildrenRemoved();
    qDeleteAll(_childItems);
    _childItems.clear();
    return true;
}

bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > _childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete _childItems.takeAt(position);

    return true;
}

TreeItem *TreeItem::child(int row)
{
    return _childItems.value(row);
}

int TreeItem::childCount() const
{
    return _childItems.count();
}

bool TreeItem::hasChildren() const
{
    return !_childItems.isEmpty();
}

int TreeItem::columnCount() const
{
    return _itemData.count();
}

QVariant TreeItem::data(int column) const
{
    load();
    return _itemData.value(column);
}

bool TreeItem::hasStream() const
{
    return false;
}

QVariant TreeItem::clipboardValue() const
{
    return QVariant();
}

TreeItem *TreeItem::parent()
{
    return _parentItem;
}

void TreeItem::load() const
{
    if(!_loaded)
    {
        doLoad();
        _loaded = true;
    }
}

int TreeItem::row() const
{
    if (_parentItem)
        return _parentItem->_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

bool TreeItem::synchronised()
{
    return true;
}

QList<QVariant> &TreeItem::itemData() const
{
    return _itemData;
}

void TreeItem::doLoad() const
{
}

void TreeItem::onChildrenRemoved()
{
}


TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent, QObject *owner)
       :QObject(owner),
        _parentItem(parent),
        _itemData(data),
        _loaded(false)
{
    if(parent != nullptr)
        parent->appendChild(this);
}
