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

#define FILE_SLOTS 512

#include <QMainWindow>
#include <QFileDialog>
#include <QDockWidget>
#include <QMenuBar>
#include <QAction>
#include "treewidget.h"
#include "htmldelegate.h"
#include "hexfilewidget.h"
#include "hexfiledockwidget.h"


#include "formatdetector.h"



class ModuleLoader;
class Interpreter;
/*!
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(const ModuleLoader& moduleLoader, const Interpreter& interpreter, QWidget *parent = 0);
    void openFile(const std::string& path);

protected slots:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
private slots:
    void open();
    void openRecentFile();
    void updateRecentFileActions();

private:
    void openFiles(QStringList paths);
    void createActions();
    void createMenus();


    TreeWidget* treeWidget;
    HexFileWidget* hexFileWidget;

    QMenu *fileMenu;
    QAction *openAct;
    QMenu *recentFilesMenu;
    QAction *separatorAct;

    File files[FILE_SLOTS];
    int nfiles;

    const ModuleLoader& moduleLoader;
    const Interpreter& interpreter;

    static const int maxRecentFiles = 5;
    QAction *recentFileActs[maxRecentFiles];
};

#endif // MAINWINDOW_H
