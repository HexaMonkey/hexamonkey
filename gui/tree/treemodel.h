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

#ifndef DATATREEMODEL_H
#define DATATREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QMap>

#include "core/object.h"
#include "core/modules/hmc/hmcmodule.h"
#include "gui/tree/treefileitem.h"
#include "gui/tree/treeview.h"

class TreeItem;
class ProgramLoader;
class ParsingQueue;

/**
 * @brief Model managing the data structure of the tree
 *
 * The items populating the tree are instances of TreeObjectItem
 * and is directly linked with an instance of Object. The exception
 * being the root item which is merely an instance of TreeItem.
 *
 * The model uses lazy population. This means that the model waits
 * that a node get expanded to populate its children. This also
 * means that when there is a large number of children only a limited
 * number are added, and the rest is then added progressively.
 */

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(const QString &data, const ProgramLoader& programLoader, TreeView* view, QObject *parent = 0);

    TreeItem& item(const QModelIndex &index) const;
    TreeItem* currentTreeFileItem();
    TreeView* view;

    virtual QVariant data(const QModelIndex &index, int role) const final;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const final;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const final;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const final;
    virtual QModelIndex parent(const QModelIndex &index) const final;
    virtual int realRowCount(const QModelIndex &parent = QModelIndex()) const final;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const final;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const final;

    QModelIndex addFile(File* file, const Module &module);
    virtual bool removeRows(int position, int rows, const QModelIndex &parent) override;

    void removeItem(QModelIndex index);

    void populate(const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries);

    QString rootPath();
    QString path(QModelIndex index) const;
    quint64 position(QModelIndex index) const;
    quint64 size    (QModelIndex index) const;

    void updateByFilePosition(quint64 pos);

public slots:
    void requestExpansion(const QModelIndex &index);
    void updateCurrent(const QModelIndex &index);
    void deleteChildren(const QModelIndex &index);
    void updateFilter(QString expression);
    void updateChildren(const QModelIndex &index);
    void onParsingStarted(int i);
    void onParsingFinished(int i);

signals:
    void parsingStarted(QModelIndex);
    void parsingFinished(QModelIndex);
    void filterChanged(QString);
    void invalidFilter();
    void work();


private:
    QModelIndex findItemChildByFilePosition(const TreeItem& treeItem, qint64 position);

    static const int defaultPopulation   = 64;
    static const int minPopulationRatio  = 2;
    static const int populationTries     = 32;
    QModelIndex addObject(Object &object, const QModelIndex &parent);

    TreeItem *rootItem;
    QModelIndex current;
    const ProgramLoader& programLoader;
    ParsingQueue* parsingQueue;

    QMap<int, QModelIndex> parsingIds;
};

#endif // TREEMODEL_H
