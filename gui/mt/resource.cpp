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

