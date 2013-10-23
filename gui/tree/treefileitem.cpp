#include "gui/tree/treefileitem.h"

TreeFileItem::TreeFileItem(const ProgramLoader &programLoader, TreeItem *parent)
    : TreeObjectItem(programLoader, parent)
{
}

void TreeFileItem::setObjectMemory(Object *object)
{
    _objectMemory.reset(object);
    setObject(*object);
}

File &TreeFileItem::file()
{
    return _file;
}
