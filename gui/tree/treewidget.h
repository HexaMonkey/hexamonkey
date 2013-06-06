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

#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeView>
#include <QGridLayout>
#include <QString>

#include "treemodel.h"
#include "treeview.h"
#include "htmldelegate.h"
#include "filterwidget.h"
#include "resourcemanager.h"

class InterpreterConstructor;
/*!
 * @brief The TreeWidget class
 */
class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(const InterpreterConstructor& interpreterConstructor, QWidget *parent = 0);
    QSize sizeHint() const;
    void setColumnsWidths();

signals:
    void pathChanged(QString);
    void positionChanged(qint64, qint64);
    void eventDropped(QDropEvent*);

public slots:
    QModelIndex addObject(Object& object);
    void updatePath(QModelIndex currentIndex);
    void updatePosition(QModelIndex currentIndex);
    void setCurrentIndex(QModelIndex index);

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);

private:
    TreeView* view;
    TreeModel* model;
    HTMLDelegate* delegate;
    FilterWidget* filterWidget;

    const InterpreterConstructor& interpreterConstructor;

    QString path;
    quint64 position;
    quint64 size;
};

#endif // TREEWIDGET_H
