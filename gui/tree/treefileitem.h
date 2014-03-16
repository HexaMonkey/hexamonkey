#ifndef TREEFILEITEM_H
#define TREEFILEITEM_H

#include <memory>

#include "gui/tree/treeobjectitem.h"

/**
 * @brief Tree Item that represent the root of a file
 *
 * The object holds the memory of the \link File file\endlink
 * and its corresponding \link Object object\endlink
 */
class TreeFileItem : public TreeObjectItem
{
public:
    TreeFileItem(const ProgramLoader& programLoader, TreeItem *parent);

    void setObjectMemory(Object *object);
    File& file();

private:
    std::unique_ptr<Object> _objectMemory;
    RealFile _file;
};

#endif // TREEFILEITEM_H
