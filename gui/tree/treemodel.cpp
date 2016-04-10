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

#include <QMessageBox>

#include "core/modules/default/defaultmodule.h"
#include "gui/tree/treemodel.h"
#include "gui/tree/treeitem.h"
#include "gui/tree/treeobjectitem.h"
#include "gui/thread/threadqueue.h"

TreeModel::TreeModel(const QString &/*data*/, const ProgramLoader &programLoader, TreeView* view, QObject *parent) :
    QAbstractItemModel(parent),
    view(view),
    programLoader(programLoader),
    threadQueue(new ThreadQueue(this))
{
    QList<QVariant> rootData;
    rootData << "Struct" << "Beginning position" << "Size";
    rootItem = TreeItem::RootItem(rootData, this);

    connect(threadQueue, SIGNAL(started(int)), this, SLOT(onThreadStarted(int)));
    connect(threadQueue, SIGNAL(finished(int)), this, SLOT(onThreadFinished(int)));
}

TreeItem &TreeModel::item(const QModelIndex &index) const
{
    if(index.isValid())
        return *static_cast<TreeItem*>(index.internalPointer());
    else
        return *rootItem;
}

TreeItem* TreeModel::currentTreeFileItem(){
    return rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return item(parent).columnCount();
}

QModelIndex TreeModel::addFile(File* file, const Module& module)
{
    beginInsertRows(QModelIndex(),0,0);

    TreeFileItem& item = *(new TreeFileItem(programLoader, rootItem, file));
    item.setObjectMemory(module.handleFile(module.getTemplate("File")(), item.file(), item.collector()));

    QModelIndex itemIndex = index(realRowCount(QModelIndex())-1, 0, QModelIndex());

    endInsertRows();
    return itemIndex;
}

QModelIndex TreeModel::currentFileIndex() const
{
    QModelIndex fileIndex = current;

    while (fileIndex.parent().column() != -1) {
        fileIndex = fileIndex.parent();
    }

    return fileIndex;
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

    int count = realRowCount(parent);

    if (count == 0 && (!object.parsed() || object.numberOfChildren())) {
        count = 1;
    }

    return count;
}

QModelIndex TreeModel::addObject(Object& object, const QModelIndex &parent)
{
    new TreeObjectItem(object, programLoader, &item(parent));

    QModelIndex itemIndex = index(realRowCount(parent)-1, 0, parent);

    return itemIndex;
}



void TreeModel::updateChildren(const QModelIndex& index)
{
    TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());
    int count = 0;
    int first = realRowCount(index);

    for (Object::iterator it = item.nextChild(); it != item.end(); ++it) {

        Object* object = *it;
        if (object != nullptr) {
            if (item.filterObject(*object)) {
                addObject(*object, index);
                count++;
            }
        }
        item.advanceLastChild();
    }

    beginInsertRows(index, first, first+count);
    endInsertRows();

    if (count) {
        emit dataChanged(index.child(0,0), index.child(count-1,columnCount(index)-1));
    }
}

void TreeModel::onThreadStarted(int i)
{
    auto parsingIt = parsingIds.find(i);
    if (parsingIt != parsingIds.end()) {
        QModelIndex index = *parsingIt;

        if (index.isValid()) {
            emit parsingStarted(index);
        }
    }

    auto exploringIt = exploringIds.find(i);
    if (exploringIt != exploringIds.end()) {
        auto item = *exploringIt;

        emit exploringStarted(std::get<0>(item), std::get<1>(item));
    }
}

void TreeModel::onThreadFinished(int i)
{
    auto parsingIt = parsingIds.find(i);
    if (parsingIt != parsingIds.end()) {
        QModelIndex index = parsingIds.take(i);

        if (index.isValid()) {
            TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());

            item.setSynchronising(false);
            updateChildren(index);

            emit parsingFinished(index);
        }
    }

    auto exploringIt = exploringIds.find(i);
    if (exploringIt != exploringIds.end()) {
        auto item = exploringIds.take(i);

        findItemChildByFilePosition(std::get<0>(item), std::get<1>(item), std::get<2>(item));

        emit exploringFinished(std::get<0>(item), std::get<1>(item));
    }
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

void TreeModel::populate(const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries)
{
    TreeObjectItem& item = *static_cast<TreeObjectItem*>(index.internalPointer());
    Object& object = item.object();

    if (item.object().parsed()) {
        if (!item.synchronised()) {
            updateChildren(index);
        }
    } else {
        if (!item.synchronising()) {
            item.setSynchronising(true);
            threadQueue->add([&object, nominalCount, minCount, maxTries] {
                VariableCollectionGuard guard(object.collector());

                int minNumberOfChildren = object.numberOfChildren() + minCount;

                for (unsigned int tries = 0;
                     object.numberOfChildren() < minNumberOfChildren && !object.parsed() && tries < maxTries;
                     ++tries) {
                     object.exploreSome(nominalCount);
                }
            }, [this, &index] (int id) {
                parsingIds.insert(id, index);
            });
        }
    }
}

void TreeModel::updateCurrent(const QModelIndex &index)
{
    current = index;
    if(current.isValid()) {
        TreeItem& currentItem = *static_cast<TreeItem*>(current.internalPointer());
        filterChanged(QString(static_cast<TreeObjectItem&>(currentItem).filterExpression().c_str()));
        if(current.parent().isValid()) {
            TreeItem* parentItem = static_cast<TreeItem*>(current.parent().internalPointer());

            if (!parentItem->synchronised()) {
                int row = currentItem.row();
                int totalRowCount = realRowCount(current.parent());
                if (row>=totalRowCount-defaultPopulation/minPopulationRatio) {
                    populate(current.parent(), defaultPopulation, defaultPopulation/minPopulationRatio, populationTries);
                }
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

int TreeModel::findItemChildByFilePosition(const QModelIndex &index, qint64 bytePos, std::function<void (const QList<size_t>&)> resultCallback)
{
    Object* object = &static_cast<TreeObjectItem*>(index.internalPointer())->object();

    qint64 bitPos = 8*bytePos;

    if (object->includesPos(bitPos)) {
        Object* currentObject = object;
        QList<size_t> result;
        bool success = false;

        while (true) {
            if (currentObject->parsed() && currentObject->numberOfChildren() == 0) {
                success = true;
                break;
            } else {
                bool found = false;
                for (int i = 0; true; ++i) {
                    Object* child = currentObject->access(i);
                    if (child == nullptr) {
                        break;
                    } else if (child->includesPos(bitPos)) {
                        currentObject = child;
                        found = true;
                        result.append(i);
                        break;
                    }
                }

                if (!found) {
                    if (currentObject->parsed()) {
                        success = true;
                    }
                    break;
                }
            }
        }

        if (success) {
            resultCallback(result);
        } else {
            threadQueue->add([object, bitPos] {
                VariableCollectionGuard guard(object->collector());

                Object* currentObject = object;
                while (true) {
                    if (currentObject->parsed() && currentObject->numberOfChildren() == 0) {
                        break;
                    } else {
                        bool found = false;
                        for (int i = 0; true; ++i) {
                            Object* child = currentObject->access(i, true);
                            if (child == nullptr) {
                                break;
                            } else if (child->includesPos(bitPos)) {
                                currentObject = child;
                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            break;
                        }
                    }
                }
            }, [this, &index, &bytePos, &resultCallback] (int id) {
                exploringIds.insert(id, std::make_tuple(index, bytePos, resultCallback));
            });

        }
    }

    return 0;
}

QString TreeModel::rootPath(){
    QModelIndex rootIndex = current;
    while(rootIndex.parent().column() != -1)
    {
        rootIndex = rootIndex.parent();
    }
    return path(index(rootIndex.row(),0));
}
