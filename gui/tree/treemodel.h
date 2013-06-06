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

#include "hmcmodule.h"
#include "object.h"
#include "resourcemanager.h"


class TreeItem;
class InterpreterConstructor;
/*!
 * @brief The DataTreeModel class
 */
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(const QString &data, const InterpreterConstructor& interpreterConstructor, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int realRowCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex addObject(Object &object);

    int populate(const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries);

    QString path(QModelIndex index) const;
    quint64 position(QModelIndex index) const;
    quint64 size    (QModelIndex index) const;

    void addResource(Object& object);

public slots:
    void requestExpansion(const QModelIndex &index);
    void updateCurrent(const QModelIndex &index);
    void deleteChildren(const QModelIndex &index);
    void updateFilter(QString expression);
    int updateChildren(const QModelIndex& index);

signals:
    void filterChanged(QString);
    void invalidFilter();
    void work();


private:
    static const int defaultPopulation   = 128;
    static const int minPopulationRatio  = 2;
    static const int populationTries     = 32;
    QModelIndex addObject(Object &object, const QModelIndex &parent);

    TreeItem *rootItem;
    QModelIndex current;
    const InterpreterConstructor& interpreterConstructor;
    ResourceManager _resourceManager;
};

#endif // TREEMODEL_H
