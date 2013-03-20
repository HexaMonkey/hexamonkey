#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <QObject>
#include <QThread>

#include "worker.h"
#include "tasklist.h"

class ResourceManager;
class TreeModel;

class Resource : public QObject
{
    Q_OBJECT
public:
    Resource(TreeModel& model);

    void lock();

    void unlock();

    TaskList& taskList();

private:
    TaskList _taskList;
    Worker _worker;
    QThread _thread;
    QMutex _mutex;

};

#endif // RESSOURCE_H
