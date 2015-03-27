#include "gui/thread/threadqueue.h"
#include "gui/thread/functionthread.h"

#include <QThread>


ThreadQueue::ThreadQueue(QObject *parent)
    :QObject(parent),
     _currentThread(nullptr),
     _currentId(0),
     _freeId(1)
{
}

void ThreadQueue::onThreadFinished()
{
    emit finished(_currentId);
    delete _currentThread;
    _currentThread = nullptr;
    _currentId = 0;
    onQueueUpdated();
}

void ThreadQueue::onQueueUpdated()
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


void ThreadQueue::add(QThread *thread, const std::function<void(int)> &registerId)
{
    _threadQueue.enqueue(std::make_pair(_freeId, thread));
    registerId(_freeId++);
    onQueueUpdated();
}

void ThreadQueue::add(std::function<void ()> functionToRun, const std::function<void (int)> &registerId)
{
    add(new FunctionThread(this, functionToRun), registerId);
}
