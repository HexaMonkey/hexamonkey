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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem : public QObject
{
    Q_OBJECT
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent);
    static TreeItem* RootItem(const QList<QVariant> &data, QObject *owner);
    virtual ~TreeItem() {}

    void removeChildren();

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parent();

    void load() const;
    virtual bool synchronised();

protected:
    QList<QVariant>& itemData() const;
    virtual void doLoad() const;


signals:
    bool childrenRemoved();

private:
    TreeItem(const QList<QVariant> &data, TreeItem *parent, QObject *owner);
    void appendChild(TreeItem *child);

    QList<TreeItem*> _childItems;
    TreeItem* _parentItem;
    mutable QList<QVariant> _itemData;
    mutable bool _loaded;
};

#endif // TREEITEM_H
