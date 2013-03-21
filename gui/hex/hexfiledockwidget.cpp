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

#include "hexfiledockwidget.h"

HexFileDockWidget::HexFileDockWidget(QWidget *parent) :
    QDockWidget(tr("hex"), parent)
{
    setFeatures(QDockWidget::DockWidgetMovable
               |QDockWidget::DockWidgetFloatable
               |QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    resize(0,0);
}


void HexFileDockWidget::setWidget(HexFileWidget *widget)
{
    QDockWidget::setWidget(widget);
    this->widget = widget;

    connect(this,SIGNAL(focusedIn()),this->widget, SLOT(focusIn()));
    connect(this,SIGNAL(focusedOut()),this->widget, SLOT(focusOut()));
}

void HexFileDockWidget::focusInEvent (QFocusEvent *event)
{
    QDockWidget::focusInEvent(event);
    emit focusedIn();
}

void HexFileDockWidget::focusOutEvent(QFocusEvent *event)
{
    QDockWidget::focusOutEvent(event);
    emit focusedOut();
}

void HexFileDockWidget::windowActivationChange (bool oldActive)
{
    if(!oldActive)
    {
        emit focusedIn();
    }
    else
    {
        emit focusedOut();
    }
}
