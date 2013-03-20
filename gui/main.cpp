#include <QtGui>
#include <QApplication>
#include <QIcon>

#include "moduleloader.h"
#include "interpreterconstructor.h"
#include "mainwindow.h"

#include "standardmodule.h"
#include "mp4module.h"
#include "ebmlmodule.h"
#include "hmcmodule.h"
#include "mkvmodule.h"
#include "tsmodule.h"
#include "fromfilemodule.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HexaMonkey");
    QCoreApplication::setApplicationName("HexaMonkey");

    ModuleLoader moduleLoader;

    moduleLoader.addModule("bestd", new StandardModule(true));
    moduleLoader.addModule("lestd", new StandardModule(false));
    moduleLoader.addModule("mp4",   new Mp4Module);
    moduleLoader.addModule("ebml",  new EbmlModule);
    moduleLoader.addModule("mkv",   new MkvModule);
    moduleLoader.addModule("hmc",   new HmcModule);
    moduleLoader.addModule("ts",    new TsModule);

    InterpreterConstructor interpreterConstructor(static_cast<const HmcModule&>(moduleLoader.loadModule("hmc")));

#ifdef __MINGW32__
    moduleLoader.addFolder(".\\scripts\\", interpreterConstructor);
    moduleLoader.addFolder("..\\scripts\\", interpreterConstructor);
#else
    moduleLoader.addFolder("./scripts/", interpreterConstructor);
    moduleLoader.addFolder("../scripts/", interpreterConstructor);
#endif

    MainWindow window(moduleLoader, interpreterConstructor);

    window.setWindowTitle("HexaMonkey");
    window.setWindowIcon(QIcon(":/logo.svg"));
    window.showMaximized();
    
    return a.exec();
}
