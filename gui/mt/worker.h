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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QModelIndex>

class Resource;

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(Resource& resource);
    ~Worker();


public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void updateChildren( QModelIndex index);

private:
    Resource& _resource;

};
#endif // WORKER_H
