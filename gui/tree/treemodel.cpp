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

#include "core/modules/default/defaulttypes.h"
#include "gui/mt/resource.h"
#include "gui/mt/resourcemanager.h"
#include "gui/tree/treemodel.h"
#include "gui/tree/treeitem.h"
#include "gui/tree/treeobjectitem.h"

TreeModel::TreeModel(const QString &/*data*/, const ProgramLoader &programLoader, QObject *parent) :
    QAbstractItemModel(parent),
    programLoader(programLoader),
    _resourceManager(*this)
{
    QList<QVariant> rootData;
    rootData << "Struct" << "Beginning position" << "Size";
    rootItem = TreeItem::RootItem(rootData, this);
}

TreeItem &TreeModel::item(const QModelIndex &index) const
{
    if(index.isValid())
        return *static_cast<TreeItem*>(index.internalPointer());
    else
        return *rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return item(parent).columnCount();
}

QModelIndex TreeModel::addFile(const std::string &path, const Module& module)
{
    beginInsertRows(QModelIndex(),0,0);

    TreeFileItem& item = *(new TreeFileItem(programLoader, rootItem));
    item.file().setPath(path);
    item.setObjectMemory(module.handle(defaultTypes::file, item.file()));

    QModelIndex itemIndex = index(realRowCount(QModelIndex())-1, 0, QModelIndex());

    endInsertRows();
    return itemIndex;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem &parentItem = item(parent);

    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem.removeChildren(position, rows);
    endRemoveRows();

    return success;

}

void TreeModel::removeItem(QModelIndex index)
{
    QModelIndex parent = index.parent();
    int row = index.row();
    removeRow(row, parent);
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    return item(index).data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *childItem =item(parent).child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::realRowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    return item(parent).childCount();
}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return rootItem->childCount();

    Object& object = static_cast<TreeObjectItem*>(parent.internalPointer())->object();

    int realCount = realRowCount(parent);

    if(realCount == 0 && (!object.parsed() || object.numberOfChildren()))
        return 1;
    else
        return realCount;
}

QModelIndex TreeModel::addObject(Object& object, const QModelIndex &parent)
{
    new TreeObjectItem(object, programLoader, &item(parent));

    QModelIndex itemIndex = index(realRowCount(parent)-1, 0, parent);

    return itemIndex;
}



int TreeModel::updateChildren(const QModelIndex& index)
{
    TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());
    int count = 0;
    int first = realRowCount(index);
    _resourceManager.lock(item.object());
    for(Object::iterator it = item.nextChild(); it != item.end(); ++it)
    {
        Object* object = *it;
        if(object != nullptr)
        {
            if(item.filterObject(*object))
            {
                addObject(*object, index);
                count++;
            }
        }
        item.advanceLastChild();
    }
    beginInsertRows(index, first, first+count);
    endInsertRows();
    if(count)
    {
        emit dataChanged(index.child(0,0), index.child(count-1,columnCount(index)-1));
    }
    _resourceManager.unlock(item.object());
    return count;
}

void TreeModel::deleteChildren(const QModelIndex &index)
{
    const int count = realRowCount(index);
    beginRemoveRows(index,0, count);
    endRemoveRows();
    if(index.isValid())
    {
        TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());
        item.removeChildren();
        item.setLastChildIndex(0);
    }


}

void TreeModel::updateFilter(QString expression)
{
    if(current.isValid())
    {
        deleteChildren(current);
        TreeObjectItem& item = *static_cast<TreeObjectItem*>(current.internalPointer());
        if(!item.updateFilter(expression.toStdString()) && expression != "")
            emit invalidFilter();

        populate(current, defaultPopulation, defaultPopulation/minPopulationRatio, populationTries);
    }
}

void TreeModel::requestExpansion(const QModelIndex &i)
{
    if(i.isValid())
    {
        QModelIndex realIndex = index(i.row(), 0, i.parent());
        if(!static_cast<TreeItem*>(realIndex.internalPointer())->synchronised())
            populate(realIndex, defaultPopulation, defaultPopulation/minPopulationRatio, populationTries);
    }
}

int TreeModel::populate(const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries)
{
    TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());
    Object& objectData = item.object();

    unsigned int count = 0;
    unsigned int tries = maxTries;

#if 1
    while(count<minCount && (!item.object().parsed() || !item.synchronised()) && tries>0)
    {
        if(!objectData.parsed())
        {
            objectData.exploreSome(nominalCount);
        }
        count += updateChildren(index);
        --tries;
    }
#else
    _resourceManager.lock(*objectData);
    if(!objectData->parsed())
    {
        QModelIndex i = index;
        _resourceManager.resource(*objectData).taskList().addPriorityTask(*objectData, i);
        emit work();
    }
    _resourceManager.unlock(*objectData);
    //QThread::msleep(100);
    //updateChildren(index);
#endif

    return count;
}

void TreeModel::updateCurrent(const QModelIndex &index)
{
    current = index;
    if(current.isValid())
    {
        TreeItem& currentItem = *static_cast<TreeItem*>(current.internalPointer());
        filterChanged(QString(static_cast<TreeObjectItem&>(currentItem).filterExpression().c_str()));
        if(current.parent().isValid())
        {
            TreeItem* parentItem = static_cast<TreeItem*>(current.parent().internalPointer());

            if(!parentItem->synchronised())
            {
                int row = currentItem.row();
                int totalRowCount = realRowCount(current.parent());
                if(row>=3*totalRowCount/4)
                    populate(current.parent(), totalRowCount, totalRowCount/minPopulationRatio, populationTries);
            }
        }
    }
}

QString TreeModel::path(QModelIndex index) const
{
    Object& object = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return QString(object.file().path().c_str());
}

quint64 TreeModel::position(QModelIndex index) const
{
    Object& object = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return object.beginningPos();
}

quint64 TreeModel::size(QModelIndex index) const
{
    Object& object = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return object.size();
}

void TreeModel::addResource(Object &object)
{
    _resourceManager.insert(object.file());
}
