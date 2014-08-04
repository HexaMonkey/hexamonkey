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

#include "gui/tree/treeview.h"
#include "gui/tree/treewidget.h"

TreeView::TreeView(TreeWidget &parent) :
    QTreeView(&parent),
    parent(parent)
{
}

void TreeView::currentChanged(const QModelIndex &current, const QModelIndex & previous)
{
    QTreeView::currentChanged(current, previous);
    if(current.isValid())
    {
        QModelIndex realCurrent = current.sibling(current.row(), 0);
        emit selected(realCurrent);
    }
}


void TreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        parent.copy();
    }
    else
    {
        QTreeView::keyPressEvent(event);
    }
}
