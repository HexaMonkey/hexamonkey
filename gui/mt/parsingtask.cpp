#include "parsingtask.h"

ParsingTask::ParsingTask(Object &object, QModelIndex index, int hint) : _object(object), _index(index), _hint(hint)
{
}

Object& ParsingTask::object()
{
    return _object;
}

QModelIndex ParsingTask::index()
{
    return _index;
}

int ParsingTask::hint()
{
    return _hint;
}

