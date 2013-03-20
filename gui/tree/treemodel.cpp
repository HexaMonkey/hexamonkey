#include "treemodel.h"
#include "treeitem.h"
#include "treeobjectitem.h"
#include "interpreterconstructor.h"
#include "resourcemanager.h"
#include "resource.h"

TreeModel::TreeModel(const QString &/*data*/, const InterpreterConstructor& interpreterConstructor, QObject *parent) :
    QAbstractItemModel(parent), interpreterConstructor(interpreterConstructor),
    _resourceManager(*this)
{
    QList<QVariant> rootData;
    rootData << "Struct" << "Beginning position" << "Size";
    rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
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

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
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
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return rootItem->childCount();

    Object* object = static_cast<TreeObjectItem*>(parent.internalPointer())->object();

    int realCount = realRowCount(parent);

    if(realCount == 0 && !object->parsed())
        return 1;
    else
        return realCount;
}

QModelIndex TreeModel::addObject(Object& object)
{
    beginInsertRows(QModelIndex(),0,0);
    QModelIndex index = addObject(object, QModelIndex());
    endInsertRows();
    return index;
}

QModelIndex TreeModel::addObject(Object& object, const QModelIndex &parent)
{
    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeObjectItem *item = new TreeObjectItem(object, interpreterConstructor.newInstance(), parentItem);
    parentItem->appendChild(item);

    QModelIndex itemIndex = index(realRowCount(parent)-1, 0, parent);


    //updateChildren(itemIndex);

    return itemIndex;
}



int TreeModel::updateChildren(const QModelIndex& index)
{
    TreeObjectItem* item = static_cast<TreeObjectItem*>(index.internalPointer());
    int count = 0;
    int first = realRowCount(index);
    _resourceManager.lock(*(item->object()));
    for(Object::iterator it = item->nextChild(); it != item->end(); ++it)
    {
        Object* object = *it;
        if(object != nullptr)
        {
            if(item->filterObject(*object))
            {
                addObject(*object, index);
                count++;
            }
        }
        item->advanceLastChild();
    }
    beginInsertRows(index, first, first+count);
    endInsertRows();
    if(count)
    {
        emit dataChanged(index.child(0,0), index.child(count-1,columnCount(index)-1));
    }
    _resourceManager.unlock(*(item->object()));
    return count;
}

void TreeModel::deleteChildren(const QModelIndex &index)
{
    const int count = realRowCount(index);
    beginRemoveRows(index,0, count);
    endRemoveRows();
    if(index.isValid())
    {
        TreeObjectItem* item = static_cast<TreeObjectItem*>(index.internalPointer());
        item->removeChildren();
        item->setLastChildIndex(0);
    }


}

void TreeModel::updateFilter(QString expression)
{
    if(current.isValid())
    {
        deleteChildren(current);
        TreeObjectItem* item = static_cast<TreeObjectItem*>(current.internalPointer());
        if(!item->updateFilter(expression.toStdString()) && expression != "")
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
    TreeObjectItem* item = static_cast<TreeObjectItem*>(index.internalPointer());
    Object* objectData = item->object();

    unsigned int count = 0;
    unsigned int tries = maxTries;

#if 1
    while(count<minCount && (!item->object()->parsed() || !item->synchronised()) && tries>0)
    {
        if(!objectData->parsed())
        {
            objectData->exploreSome(nominalCount);
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
        TreeItem* currentItem = static_cast<TreeItem*>(current.internalPointer());
        filterChanged(QString(static_cast<TreeObjectItem*>(currentItem)->filterExpression().c_str()));
        if(current.parent().isValid())
        {

            TreeItem* parentItem = static_cast<TreeItem*>(current.parent().internalPointer());

            if(!parentItem->synchronised())
            {
                int row = currentItem->row();
                int totalRowCount = realRowCount(current.parent());
                if(row>=3*totalRowCount/4)
                    populate(current.parent(), totalRowCount, totalRowCount/minPopulationRatio, populationTries);
            }
        }
    }
}

QString TreeModel::path(QModelIndex index) const
{
    Object* dataObject = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return QString(dataObject->file().path().c_str());
}

quint64 TreeModel::position(QModelIndex index) const
{
    Object* dataObject = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return dataObject->beginningPos();
}

quint64 TreeModel::size(QModelIndex index) const
{
    Object* dataObject = static_cast<TreeObjectItem*>(index.internalPointer())->object();
    return dataObject->size();
}

void TreeModel::addResource(Object &object)
{
    _resourceManager.insert(object.file());
}
