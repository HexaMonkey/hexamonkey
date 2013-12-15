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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include "core/formatdetector/formatdetector.h"

#include "gui/tree/treewidget.h"
#include "gui/tree/htmldelegate.h"
#include "gui/hex/hexfilewidget.h"

#include <QAction>


#include "qtprogramloader.h"
#include "core/interpreter/fromfilemodule.h"
#include "core/modules/standard/standardmodule.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/modules/mkv/mkvmodule.h"
#include "core/moduleloader.h"
#include "core/util/fileutil.h"
#include "core/util/osutil.h"


class ModuleLoader;
class ProgramLoader;

/**
 * @brief Controller for all widget
 *
 * The main window is mostly composed of two widgets :
 * the TreeWidget and the HexFileWidget
 *
 * The main window is also composed of a menu bar.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(ModuleLoader *moduleLoader, const ProgramLoader &programLoader, std::vector<std::string> scriptsDirs, QWidget *parent = 0);

    /**
     * @brief Open a file and set it as the current file in
     * the tree and hex widgets.
     */
    void openFile(const std::string& path);
    TreeWidget* getTreeWidget();

private slots:
    virtual void dropEvent(QDropEvent *event) final;
    virtual void dragEnterEvent(QDragEnterEvent* event) final;
    virtual void dragMoveEvent(QDragMoveEvent* event) final;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) final;

    void open();
    void openRecentFile();
    void updateRecentFileActions();
    void refreshment();

private:
    void openFiles(QStringList paths);
    void createActions();
    void createMenus();

    TreeWidget* treeWidget;
    HexFileWidget* hexFileWidget;

    std::vector<std::string> scriptsDirs;
    QMenu *fileMenu;
    QAction *openAct;
    QAction *refreshAct;
    QMenu *recentFilesMenu;
    QAction *separatorAct;

    ModuleLoader* moduleLoader;
    const ProgramLoader& programLoader;

    static const int maxRecentFiles = 5;
    QAction *recentFileActs[maxRecentFiles];

};

#endif // MAINWINDOW_H
