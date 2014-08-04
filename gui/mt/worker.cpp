//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <QtGlobal>
#include <QTime>

#include "gui/mt/worker.h"
#include "gui/mt/resource.h"
#include "gui/mt/tasklist.h"

Worker::Worker(Resource &resource) : _resource(resource) {
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
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
