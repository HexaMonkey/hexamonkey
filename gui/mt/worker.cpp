#include <QtGlobal>
#include <QTime>

#include "worker.h"
#include "resource.h"
#include "tasklist.h"

Worker::Worker(Resource &resource) : _resource(resource) {
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

Worker::~Worker() {
}

void Worker::process() {
    ParsingTask* next;
    while((next = _resource.taskList().nextTask()) != nullptr)
    {
        _resource.lock();
        QModelIndex index = next->index();
        next->object().exploreSome(next->hint());
        if (!next->object().parsed() && (qrand() % 100))
            _resource.taskList().addStandardTask(next->object(), 1);
        delete next;
        next = nullptr;
        if (index.isValid())
        {
            emit updateChildren(index);
        }
        _resource.unlock();
    }
}


/*
1	QThread* thread = new QThread;
2	Worker* worker = new Worker();
3	worker->moveToThread(thread);
4	connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
5	connect(thread, SIGNAL(started()), worker, SLOT(process()));
6	connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
7	connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
8	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
9	thread->start();
*/
