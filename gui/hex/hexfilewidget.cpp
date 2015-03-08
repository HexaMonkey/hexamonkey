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

#include <QtGui>
#include <QGridLayout>
#include "core/util/strutil.h"
#include "gui/hex/hexfilemodel.h"
#include "gui/hex/hexfilewidget.h"
#include "../mainwindow.h"


HexFileWidget::HexFileWidget(QWidget *parent)
    : QWidget(parent)
{
    //Data model
    model = new HexFileModel(this);

    setFocusPolicy(Qt::StrongFocus);

    //Viewport for the model
    view = new HexFileView(this);
    view->setModel(model);

    header = new HexFileHeader(view, this);
    searchWidget = new HexFileSearchWidget(this);
    
    //Layout
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(header, 0, 0, 1, 1);
    layout->addWidget(view, 1, 0, 1, 1);
    layout->addWidget(searchWidget, 2, 0, 1, 1);
    view->setContentsMargins(0,0,0,0);

    connect(view,SIGNAL(highlighted(QModelIndex)), this, SLOT(focus(QModelIndex)));
    connect(view,SIGNAL(selected(QModelIndex)), this, SLOT(select(QModelIndex)));
    connect(view,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(select(QModelIndex)));
    connect(view,SIGNAL(focusedIn()) , this, SLOT(focusIn()));
    connect(view,SIGNAL(focusedOut()), this, SLOT(focusOut()));
    connect(this,SIGNAL(focusedIn()) , this, SLOT(focusIn()));
    connect(this,SIGNAL(focusedOut()), this, SLOT(focusOut()));
    connect(searchWidget, SIGNAL(searchRequested(QByteArray,bool)), this, SLOT(search(QByteArray,bool)));

    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayMenu(QPoint)));

    setContentsMargins(0,0,0,0);

    setLayout(layout);

}

unsigned int HexFileWidget::hexWidth() const
{
    return _hexWidth;
}

unsigned int HexFileWidget::charWidth() const
{
    return _charWidth;
}

void HexFileWidget::setFile(const QString& path)
{
    delete model;
    model = new HexFileModel(this);
    model->setFile(path);
    view->setModel(model);
    view->setHeaderCharCount(toHex(model->rowCount()).size()+1);
    searchWidget->reset();
    header->repaint();
}

void HexFileWidget::gotoPosition(qint64 position)
{
    view->scrollTo(model->modelIndex(position/8), QAbstractItemView::PositionAtCenter);
}

void HexFileWidget::selectPosition(qint64 position)
{
    QModelIndex index = model->modelIndex(position/8);
    view->setCurrentIndex(index);
    view->scrollTo(index, QAbstractItemView::PositionAtCenter);
}

void HexFileWidget::highlight(qint64 position, qint64 size)
{
    quint64 oldPosition = model->hlPosition;
    quint64 oldSize = model->hlSize;
    model->hlPosition = position/8;
    model->hlSize = (size%8 == 0) ? size/8 : size/8+1;
    model->changeData(oldPosition, oldPosition+oldSize);
    model->changeData(position, position+size);
}

void HexFileWidget::focus(const QModelIndex& index)
{
    focus(model->position(index));
}

void HexFileWidget::focus(qint64 position)
{
    qint64 oldPosition = model->focusPosition;
    model->focusPosition = position;
    model->changeData(oldPosition);
    model->changeData(position);
    header->setHighlight(position%16);
}

void HexFileWidget::select(const QModelIndex &index)
{
    select(model->position(index));
}

void HexFileWidget::select(qint64 position)
{
    emit selected(position);
}

void HexFileWidget::focusInEvent (QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    emit focusedIn();
}

void HexFileWidget::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    emit focusedOut();
}

void HexFileWidget::windowActivationChange (bool oldActive)
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

void HexFileWidget::focusIn()
{
    model->focusIn();
    header->focusIn();
}

void HexFileWidget::focusOut()
{
    model->focusOut();
    header->focusOut();
}

void HexFileWidget::search(QByteArray pattern, bool next)
{
    qint64 beginningPos = model->position(view->currentIndex());
    if(next)
        ++beginningPos;
    qint64 pos = model->search(pattern, beginningPos);
    if(pos!=-1)
        selectPosition(pos);
}

void HexFileWidget::focusSearch()
{
    searchWidget->focusSearch();
}

void HexFileWidget::displayMenu(const QPoint &pos)
{
    QMenu menu(this);

    QAction *selectAction = menu.addAction("Select");

    QAction *triggeredAction = menu.exec(view->viewport()->mapToGlobal(pos));

    if(!triggeredAction)
        return;

    if(triggeredAction == selectAction)
    {
        select(view->currentIndex());
    }
}
