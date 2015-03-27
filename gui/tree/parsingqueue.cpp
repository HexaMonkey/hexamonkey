#include "gui/tree/parsingthread.h"
#include "gui/tree/parsingqueue.h"


ParsingQueue::ParsingQueue(QObject *parent)
    :QObject(parent),
     _currentThread(nullptr),
     _currentId(0),
     _freeId(1)
{
}

void ParsingQueue::addObjectParsing(Object& object, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries, const std::function<void(int)>& registerId)
{
    addThread(new ParsingThread(this, object, nominalCount, minCount, maxTries), registerId);
}

void ParsingQueue::onThreadFinished()
{
    emit finished(_currentId);
    delete _currentThread;
    _currentThread = nullptr;
    _currentId = 0;
    onQueueUpdated();
}

void ParsingQueue::onQueueUpdated()
{
    if (!_currentThread && !_threadQueue.empty()) {
        std::pair<int, QThread*> item = _threadQueue.dequeue();
        _currentId = item.first;
        _currentThread = item.second;
        connect(_currentThread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
        _currentThread->start();
        emit started(_currentId);
    }
}


void ParsingQueue::addThread(QThread *thread, const std::function<void(int)> &registerId)
{
    _threadQueue.enqueue(std::make_pair(_freeId, thread));
    registerId(_freeId++);
    onQueueUpdated();
}
