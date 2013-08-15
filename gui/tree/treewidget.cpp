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

#include "treewidget.h"

#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QFileDialog>

TreeWidget::TreeWidget(const ProgramLoader& programLoader, QWidget *parent) :
    QWidget(parent)
{
    setAcceptDrops(true);

    view =  new TreeView(*this);
    model = new TreeModel("Tree", programLoader, this);

    view->setModel(model);

    delegate = new HTMLDelegate(this);

    view->setModel(model);
    view->setItemDelegate(delegate);
    setColumnsWidths();

    filterWidget = new FilterWidget(this);

    //Layout
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(view, 0, 0, 1, 1);
    layout->addWidget(filterWidget, 1, 0, 1, 1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    connect(view, SIGNAL(selected(QModelIndex)), model, SLOT(updateCurrent(QModelIndex)));
    connect(view, SIGNAL(expanded(QModelIndex)), model, SLOT(requestExpansion(QModelIndex)));

    connect(view, SIGNAL(selected(QModelIndex)), this, SLOT(updatePath(QModelIndex)));
    connect(view, SIGNAL(selected(QModelIndex)), this, SLOT(updatePosition(QModelIndex)));

    connect(model, SIGNAL(filterChanged(QString)), filterWidget, SLOT(changeText(QString)));
    connect(filterWidget, SIGNAL(textChanged(QString)), model, SLOT(updateFilter(QString)));
    connect(model, SIGNAL(invalidFilter()), filterWidget, SLOT(invalidateText()));

    // in the constructor
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayMenu(QPoint)));
}

void TreeWidget::displayMenu(const QPoint &pos)
{
    QModelIndex current = view->currentIndex();
    if(current.isValid())
    {
        QMenu menu(this);

        QAction *copyAction = nullptr;
        if(!currentItem().clipboardValue().isNull())
            copyAction = menu.addAction("Copy Value");

        QAction *dumpToFileAction = menu.addAction("Dump to File");

        QAction *closeFileAction = menu.addAction("Close File");

        QAction *trigeredAction = menu.exec(view->viewport()->mapToGlobal(pos));

        if(!trigeredAction)
            return;

        if(trigeredAction == copyAction)
        {
            copy();
        }
        else if (trigeredAction == closeFileAction)
        {
            closeFile();
        }
        else if (trigeredAction == dumpToFileAction)
        {
            dumpToFile();
        }
    }
}

void TreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void TreeWidget::dragMoveEvent(QDragMoveEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}


void TreeWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void TreeWidget::dropEvent(QDropEvent *event)
{
    QWidget::dropEvent(event);
    std::cerr<<"eventDrop"<<std::endl;
    emit eventDropped(event);
}

QSize TreeWidget::sizeHint() const
{
    return QSize(800,800);
}

void TreeWidget::setColumnsWidths()
{
    int w = sizeHint().width();
    view->setColumnWidth(0, w - 250);
    view->setColumnWidth(1, 100);
    view->setColumnWidth(2, 120);
}

TreeItem &TreeWidget::currentItem() const
{
    return model->item(view->currentIndex());
}

void TreeWidget::copy() const
{
    QVariant value = currentItem().clipboardValue();
    if(!value.isNull())
    {
        QApplication::clipboard()->setText(value.toString());
    }
}

void TreeWidget::closeFile()
{
    QModelIndex file = view->currentIndex();
    while(file.parent().isValid())
        file = file.parent();

    setCurrentIndex(QModelIndex());
    model->removeItem(file);
}

void TreeWidget::dumpToFile()
{
    QModelIndex current = view->currentIndex();
    if(!current.isValid())
        return;

    static_cast<TreeObjectItem&>(model->item(current)).object().dumpToFile(QFileDialog::getSaveFileName(this, "Dump to file").toStdString());
}

QModelIndex TreeWidget::addFile(const std::string &path, const Module &module)
{
    return model->addFile(path, module);
}

void TreeWidget::updatePath(QModelIndex currentIndex)
{
    QString newPath = model->path(currentIndex);
    if(newPath != path)
    {
        emit pathChanged(newPath);
        path = newPath;
    }
}

void TreeWidget::updatePosition(QModelIndex currentIndex)
{
    quint64 newPosition = model->position(currentIndex);
    quint64 newSize     = model->size(currentIndex);

    if(newPosition != position || newSize != size)
    {
        emit positionChanged(newPosition, newSize);
        position = newPosition;
        size     = newSize;
    }
}

void TreeWidget::setCurrentIndex(QModelIndex index)
{
    return view->setCurrentIndex(index);
}
