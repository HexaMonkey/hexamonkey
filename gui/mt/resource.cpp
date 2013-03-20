#include <QModelIndex>

#include "resource.h"
#include "resourcemanager.h"
#include "treemodel.h"


Resource::Resource(TreeModel &model) : _worker(*this)
{
    QThread* thread = &_thread;
    Worker* worker = &_worker;
    worker->moveToThread(thread);
    //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    //connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(worker, SIGNAL(updateChildren(QModelIndex)),&model,SLOT(updateChildren(QModelIndex)));
    connect(&model, SIGNAL(work()),worker,SLOT(process()));
    thread->start();
}

void Resource::lock()
{
    _mutex.lock();
}

void Resource::unlock()
{
    _mutex.unlock();
}

TaskList &Resource::taskList()
{
    return _taskList;
}

