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

#include "object.h"
#include "treeitem.h"
#include "filter.h"

/*!
 * @brief The TreeObjectItem class
 */
class TreeObjectItem : public TreeItem
{
    Q_OBJECT
public:
    TreeObjectItem(Object& object, const Interpreter& interpreter, TreeItem *parent);
    Object& object() const;
    Object::iterator nextChild();
    Object::iterator end();
    virtual bool synchronised();

    void advanceLastChild();
    void setLastChildIndex(int64_t l);
    bool filterObject(Object &object);
    bool updateFilter(const std::string& expression);
    const std::string& filterExpression();


private slots:
    void onChildrenRemoved();

private:
   void doLoad() const override;

   Object& _object;
   int64_t _index;
   Filter filter;
   bool _synchronised;
};

#endif // DATATREEITEM_H
