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
class InterpreterConstructor;
/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(const ModuleLoader& moduleLoader, const InterpreterConstructor& interpreterConstructor, QWidget *parent = 0);

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
    void openFile(const std::string& path);
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
    const InterpreterConstructor& interpreterConstructor;

    static const int maxRecentFiles = 5;
    QAction *recentFileActs[maxRecentFiles];
};

#endif // MAINWINDOW_H
