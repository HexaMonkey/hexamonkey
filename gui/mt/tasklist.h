#ifndef TASKLIST_H
#define TASKLIST_H

#include <QStack>
#include <QQueue>
#include <QMutex>


#include "parsingtask.h"

class TaskList
{
public:
    TaskList();

    ParsingTask *nextTask();

    void addPriorityTask(Object& object,  QModelIndex index , int priority = 5 );

    void addStandardTask(Object& object, int count ,  QModelIndex index = QModelIndex() );

protected:

private:
    QStack<ParsingTask*> _task;
    QQueue<ParsingTask*> _priorityTask;
    QMutex _mutex;
};


#endif // TASKLIST_H
