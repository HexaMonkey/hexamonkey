#include <sstream>

#include "gui/tree/parsingthread.h"
#include "gui/tree/parsingqueue.h"

#include "core/object.h"

#include "core/log/logmanager.h"

ParsingThread::ParsingThread(ParsingQueue *parent,
                             Object &object,
                             unsigned int nominalCount,
                             unsigned int minCount,
                             unsigned int maxTries)
    : QThread(parent),
      _object(object),
      _nominalCount(nominalCount),
      _minCount(minCount),
      _maxTries(maxTries)
{
}

void ParsingThread::run()
{
    int minCount = _object.numberOfChildren() + _minCount;

    for (unsigned int tries = 0;
         _object.numberOfChildren() < minCount && !_object.parsed() && tries < _maxTries;
         ++tries) {
        _object.exploreSome(_nominalCount);
    }
}
