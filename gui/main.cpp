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

/**
 *\mainpage HexaMonkey developper documentation
 *
 * This is a documentation for those who wish to contribute to the HexaMonkey software.
 * For those who wish to write or modify HMScript the documentation can be found
 * <a href="../hmdoc.php">here</a>.
 *
 * The code is written in C++11 and is released under the GPLv2 licence. It is divided into
 * two main blocks : the core and the gui. The core is responsible for all file manipulation
 * and object parsing and the gui is reponsible for the Qt graphical user interface.
 *
 * The core has no library dependencies other than the standard library and rapidxml
 * that is directly included in the code. It can be included into any project to parse files.
 *
 * More details on the architecture of HexaMonkey <a href="../archdoc.php">here</a>.
 *
 * The gui is written using Qt 5.0. It is currently divided into two main widget : the \link TreeWidget tree
 * widget \endlink and the \link HexFileWidget hexadecimal widget \endlink. The tree widget is the main
 * controller, it allows to parse files and explore the structure. The hexadecimal widget display the current
 * file in hexadecimal form and in ASCII form, the memory area corresponding to the node selected in the tree
 * view is highlighted.
 */


#include <QtGui>
#include <QApplication>
#include <QIcon>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    std::cout<<std::endl;
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HexaMonkey");
    QCoreApplication::setApplicationName("HexaMonkey");



#if defined(PLATFORM_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string installDir = std::string(buffer);
    std::string::size_type pos = installDir.find_last_of( "\\/" );
    installDir = installDir.substr( 0, pos)+"\\";

    std::string userDir = installDir+"scripts\\";

    std::vector<std::string> modelsDirs = {installDir, "..\\models\\"};
    std::vector<std::string> scriptsDirs = {installDir+"scripts\\", "..\\scripts\\"};
    std::vector<std::string> compilerDirs = {installDir, "..\\compiler\\"};
    std::vector<std::string> logoDirs = {installDir, "..\\logo\\"};

#elif defined(PLATFORM_LINUX)
    /* XXX: use autotools ? */

    std::string installDir = "/usr/share/hexamonkey/";
    std::string userDir = std::string(getenv("HOME"))+"/.hexamonkey/";
    QDir::root().mkdir(QString(userDir.c_str()));

    std::vector<std::string> modelsDirs = {installDir, "../models/"};
    std::vector<std::string> scriptsDirs = {installDir+"scripts/", userDir, "../scripts/"};
    std::vector<std::string> compilerDirs = {"/usr/bin/", "../compiler/"};
    std::vector<std::string> logoDirs = {installDir, "../logo/"};

#else
    std::cerr<<"Error: unsuported operating system"<<std::endl;
    return 0;
#endif

    ModuleLoader moduleLoader;

    moduleLoader.addModule("bestd", new StandardModule(true));
    moduleLoader.addModule("lestd", new StandardModule(false));
    moduleLoader.addModule("ebml",  new EbmlModule);
    moduleLoader.addModule("mkv",   new MkvModule(getFile(modelsDirs, "mkvmodel.xml")));
    moduleLoader.addModule("hmc",   new HmcModule(getFile(modelsDirs, "hmcmodel.csv")));
    moduleLoader.addModule("stream",new StreamModule());

    QtProgramLoader programLoader(static_cast<const HmcModule&>(moduleLoader.getModule("hmc")), compilerDirs, userDir);

    moduleLoader.setDirectories(scriptsDirs, static_cast<const ProgramLoader&>(programLoader));

    MainWindow window(moduleLoader, static_cast<const ProgramLoader&>(programLoader));

    window.setWindowTitle("HexaMonkey");
    QIcon icon(getFile(logoDirs, "logo.svg").c_str());
    window.setWindowIcon(icon);
    window.showMaximized();

    for(int i = 1; i < argc; ++i)
    {
        window.openFile(argv[i]);
    }

    return a.exec();
}
