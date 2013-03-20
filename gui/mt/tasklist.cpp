#include "tasklist.h"

TaskList::TaskList() : _task(QStack<ParsingTask*>()), _priorityTask(QQueue<ParsingTask*>())
{
}

ParsingTask* TaskList::nextTask()
{
    ParsingTask* result = nullptr ;
    _mutex.lock();
    if (!_priorityTask.isEmpty())
    {
        result = _priorityTask.dequeue();
    }
    else if (!_task.isEmpty())
    {
        result = _task.pop();
    }
    _mutex.unlock();
    return result;
}

void TaskList::addPriorityTask(Object& object, QModelIndex index, int priority)
{
    if (!object.parsed())
    {
        _mutex.lock();
        for (; priority > 0 ; --priority)
        {
            ParsingTask* task = new ParsingTask(object, index, 10);
            _priorityTask.enqueue(task);
        }
        _mutex.unlock();
    }
}

void TaskList::addStandardTask(Object& object, int count, QModelIndex index)
{
    if (!object.parsed())
    {
        for (; count > 0 ; --count)
        {
            ParsingTask* task = new ParsingTask(object, index);
            _mutex.lock();
            _task.push(task);
            _mutex.unlock();
        }
    }
}
