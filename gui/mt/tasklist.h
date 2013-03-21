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
