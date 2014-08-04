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

#include "gui/mt/tasklist.h"

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
