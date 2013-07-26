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

class ProgramLoader;
class TreeItem;

/**
 * @brief Widget controlling the parsing and display of files into
 * tree structures.
 *
 * The tree is managed by two main classes : the TreeModel responsible
 * for managing the data model of the tree and the TreeView responsible
 * for displaying and controlling the tree.
 *
 * The widget also contain a FilterWidget that allows to filter nodes based
 * on the evaluation of an HMScript expression on the corresponding \link
 * Object objects\endlink.
 */
class TreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TreeWidget(const ProgramLoader &programLoader, QWidget *parent = 0);
    QSize sizeHint() const;
    void setColumnsWidths();
    TreeItem& currentItem() const;
    void copy() const;
    void closeFile();

signals:
    void pathChanged(QString);
    void positionChanged(qint64, qint64);
    void eventDropped(QDropEvent*);

public slots:
    QModelIndex addFile(const std::string& path, const Module &module);
    void updatePath(QModelIndex currentIndex);
    void updatePosition(QModelIndex currentIndex);
    void setCurrentIndex(QModelIndex index);
    void displayMenu(const QPoint &pos);

private:
    virtual void dropEvent(QDropEvent *event) final;
    virtual void dragEnterEvent(QDragEnterEvent* event) final;
    virtual void dragMoveEvent(QDragMoveEvent* event) final;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) final;

    TreeView* view;
    TreeModel* model;
    HTMLDelegate* delegate;
    FilterWidget* filterWidget;

    QString path;
    quint64 position;
    quint64 size;
};

#endif // TREEWIDGET_H
