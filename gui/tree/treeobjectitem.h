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
 * \brief The TreeObjectItem class
 */
class TreeObjectItem : public TreeItem
{
    Q_OBJECT
public:
    TreeObjectItem(Object& object, Interpreter* interpreter, TreeItem *parent = 0);
    Object* object();
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
   Object* _object;
   int64_t _index;
   Filter filter;
   bool _synchronised;
};

#endif // DATATREEITEM_H
