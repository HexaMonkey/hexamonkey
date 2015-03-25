#include "gui/tree/parsingthread.h"
#include "gui/tree/parsingqueue.h"

ParsingQueue::ParsingQueue(QObject *parent)
    :QObject(parent),
     _currentThread(nullptr)
{
}

void ParsingQueue::addObjectParsing(Object &object, const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries)
{
    ParsingThread* thread = new ParsingThread(this, object, index, nominalCount, minCount, maxTries);
    _threadQueue.enqueue(thread);
    onQueueUpdated();
}

void ParsingQueue::onThreadFinished()
{
    emit finished(_currentThread->index());
    delete _currentThread;
    _currentThread = nullptr;
    _threadQueue.dequeue();
    onQueueUpdated();
}

void ParsingQueue::onQueueUpdated()
{
    if (!_currentThread && !_threadQueue.empty()) {
        _currentThread = _threadQueue.back();
        connect(_currentThread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
        _currentThread->start();
    }
}
