#include "parsingqueue.h"

ParsingQueue::ParsingQueue(QObject *parent)
    :QObject(parent)
{
}

ParsingThread *ParsingQueue::addObjectParsing(Object &object, const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries)
{
    return nullptr;
}
