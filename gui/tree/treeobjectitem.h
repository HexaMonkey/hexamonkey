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

#ifndef TREEOBJECTITEM_H
#define TREEOBJECTITEM_H

#include <QList>
#include <QVariant>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <set>

#include "core/object.h"
#include "core/interpreter/filter.h"
#include "gui/tree/treeitem.h"

/**
 * @brief \link TreeItem Tree item\endlink linked with an \link Object object\endlink
 *
 * The children of the item are mapped to the children of the object.
 * The item is responsible for parsing the object if necessary.
 *
 * The item also holds a \link Filter filter\endlink that it can use to filter its children
 * when requested.
 */
class TreeObjectItem : public TreeItem
{
    Q_OBJECT
public:
    TreeObjectItem(Object& object, const ProgramLoader& programLoader, TreeItem *parent);
    Object& object() const;
    Object::iterator nextChild();
    Object::iterator end();
    virtual bool synchronised();

    void advanceLastChild();
    void setLastChildIndex(int64_t l);
    bool filterObject(Object &object);
    bool updateFilter(const std::string& expression);
    const std::string& filterExpression();
    virtual bool hasStream() const override;
    virtual QVariant clipboardValue() const override;

protected:
    TreeObjectItem(const ProgramLoader& programLoader, TreeItem *parent);
    virtual void onChildrenRemoved() override;
    void setObject(Object& object);

private:
   void doLoad() const override;
   Object* _object;
   int64_t _index;
   Filter filter;
   bool _synchronised;
};

#endif // DATATREEITEM_H
