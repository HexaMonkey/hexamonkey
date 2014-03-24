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

#include <QMessageBox>
#include <QHBoxLayout>
#include <QApplication>

#include "core/moduleloader.h"
#include "core/interpreter/programloader.h"
#include "core/modules/default/defaulttypes.h"

#include "mainwindow.h"


const int MainWindow::maxRecentFiles;

MainWindow::MainWindow(ModuleLoader &moduleLoader, const ProgramLoader &programLoader, QWidget *parent)
    : QMainWindow(parent),
      moduleLoader(moduleLoader),
      programLoader(programLoader)
{
    createActions();
    createMenus();
    setAcceptDrops(true);

    treeWidget = new TreeWidget(programLoader, this);
    hexFileWidget = new HexFileWidget(this);

    setCentralWidget(new QWidget(this));
    QHBoxLayout* layout = new QHBoxLayout(centralWidget());

    layout->addWidget(treeWidget, 1);
    layout->addWidget(hexFileWidget);
    layout->setContentsMargins(0,0,0,0);
    centralWidget()->setLayout(layout);

    QAction* search = new QAction(this);
    search->setShortcut(QKeySequence::Find);
    addAction(search);

    errorManager = ErrorManager::getInstance();
    errorWindow = new ErrorWindow();
    errorManager->attach(errorWindow);

    connect(treeWidget,SIGNAL(pathChanged(QString)), hexFileWidget, SLOT(setFile(QString)));
    connect(treeWidget,SIGNAL(positionChanged(qint64, qint64)), hexFileWidget, SLOT(gotoPosition(qint64)));
    connect(treeWidget,SIGNAL(positionChanged(qint64, qint64)), hexFileWidget, SLOT(highlight(qint64,qint64)));
    connect(treeWidget,SIGNAL(eventDropped(QDropEvent*)),this, SLOT(dropEvent(QDropEvent*)));
    connect(search, SIGNAL(triggered()), hexFileWidget, SLOT(focusSearch()));
    connect(treeWidget,SIGNAL(openFragmentedFile(Object&)), this, SLOT(openFragmentedFile(Object&)));
}


void MainWindow::open()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open File"));
    openFiles(paths);
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString().toStdString());
}

void MainWindow::openFiles(QStringList paths)
{
    for(int i = 0; i < paths.size(); ++i)
    {
        openFile(paths[i].toStdString());
    }
}

void MainWindow::openFile(const std::string& path)
{
    RealFile* file = new RealFile();
    file->setPath(path);
    if (!file->good())
    {
       errorManager->errorMessage << "File not found";
       errorManager->notify();
    }
    else
    {
        //Add File to recent file list
        QString qPath = path.c_str();
        QSettings settings;
        QStringList fileList = settings.value("recentFileList").toStringList();
        fileList.removeAll(qPath);
        fileList.prepend(qPath);
        while (fileList.size() > maxRecentFiles)
            fileList.removeLast();
        settings.setValue("recentFileList", fileList);
        updateRecentFileActions();

        //Create new tree node
        const Module& module = moduleLoader.getModule(*file);
        treeWidget->setCurrentIndex(treeWidget->addFile(file, module));
    }
}

void MainWindow::openFragmentedFile(Object& object) {
    FragmentedFile* fragFile = StreamModule::getFragmentedFile(&object);
    if(fragFile) {
        std::string moduleName = StreamModule::getFragmentedModule(&object);
        const Module& module = moduleLoader.getModule(moduleName);
        treeWidget->setCurrentIndex(treeWidget->addFile(fragFile, module));
    } else {
        ErrorManager::getInstance()->notify("FragmentedFile not found");
    }
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    for (int i = 0; i < maxRecentFiles; ++i)
    {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
    }
    refreshAct = new QAction(tr("Recompile scripts"), this);
    refreshAct->setShortcuts(QKeySequence::Refresh);
    connect(refreshAct, SIGNAL(triggered()), this, SLOT(refreshScripts()));
}

void MainWindow::refreshScripts()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );
    QStringList filesOpened;
    int nbFilesOpened = treeWidget->getModel()->rowCount();
    for (int i = 0; i < nbFilesOpened; ++i){
        QModelIndex index = treeWidget->getModel()->index(0, 0, QModelIndex());
        filesOpened << treeWidget->getModel()->path(index);
        treeWidget->getModel()->removeItem(index);
    }
    moduleLoader.refreshDirectories(programLoader);
    openFiles(filesOpened);
    QApplication::restoreOverrideCursor();
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(refreshAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < maxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    updateRecentFileActions();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}


void MainWindow::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files
        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }

        // call a function to open the files
        openFiles(pathList);
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList fileList = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(fileList.size(), maxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo(fileList[i]).fileName());
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(fileList[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < maxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

#if 0
void MainWindow::setupDetectors()
{
    //Magic Number Detector
    magicDetector.addMagicNumber("mp4", "00 00 00 xx 66 74 79 70");
    magicDetector.addMagicNumber("ebml", "1a 45 df a3");
    magicDetector.addMagicNumber("mkv", "1a 45 df a3 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 6d 61 74 72 6f 73 6b 61");
    magicDetector.addMagicNumber("hmc", "1a 45 df a3 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 68 65 78 61 6d 6f 6e 6b 65 79");
    magicDetector.addMagicNumber("avi", "52 49 46 46 xx xx xx xx 41 56 49 20 4c 49 53 54");
    magicDetector.addMagicNumber("asf", "30 26 B2 75 8E 66 CF 11 A6 D9 00 AA 00 62 CE 6C");


    compositeDetector.addDetector(magicDetector);

    //Syncbyt Detector
    syncbyteDetector.addFormat("ts", 0x47, 188);

    compositeDetector.addDetector(syncbyteDetector);

    //Extension Detector
    extensionDetector.addExtension("mp4", "mp4");
    extensionDetector.addExtension("mp4", "mov");
    extensionDetector.addExtension("mp4", "m4a");
    extensionDetector.addExtension("mkv", "mkv");
    extensionDetector.addExtension("ts",  "ts");
    extensionDetector.addExtension("hmc", "hmc");
    extensionDetector.addExtension("avi", "avi");
    extensionDetector.addExtension("asf", "asf");

    compositeDetector.addDetector(extensionDetector);
}
#endif
