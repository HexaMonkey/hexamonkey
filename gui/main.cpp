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
 * <a href="http://www.hexamonkey.com/hmdoc.php">here</a>.
 *
 * The code is written in C++11 and is released under the GPLv2 licence. It is divided into
 * two main blocks : the core and the gui. The core is responsible for all file manipulation
 * and object parsing and the gui is reponsible for the Qt graphical user interface.
 *
 * The core has no library dependencies other than the standard library and rapidxml
 * that is directly included in the code. It can be included into any project to parse files.
 *
 * The main class in the core is the \link ModuleLoader Module\endlink which assigns \link Module
 * modules\endlink to \link File file\endlink to parse them. The modules
 * are responsible for generating \link Parser parsers\endlink that decompose a file
 * into tree structure with the nodes being instances of the class Object. Modules
 * can be imported into others so that generic parsers can be used by several
 * \link Module modules\endlink such as integers or strings.
 *
 * The \link Module modules\endlink can either be native or HMScript. The native modules are subclasses
 * of the virtual Module class. These include the default module, which is imported by every module
 * automatically and provides basic structures such as the basic file, the array... and the standard
 * module which provides basic atoms such as integers, floats, strings..., this module exist under two
 * version : the little endian version (codename lestd) and the big endian version (codename). To create
 * a native module the most pratical class to reimplement is the MapModule. However it is recommended
 * when possible to write a new module as a HMScript.
 *
 * HMScript modules are written in a files and are interpreted by the class FromFileModule. The files are
 * compiled with an external compiler using the tools Flex and Bison in charge of lexing and parsing. The
 * compiler outputs in a special format that implements the EBML specifications. The HMScript language can
 * also be used on the fly to evaluate expression. This is for instance used by the Filter class.
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

#include "moduleloader.h"
#include "programloader.h"
#include "mainwindow.h"

#include "standardmodule.h"
#include "ebmlmodule.h"
#include "hmcmodule.h"
#include "mkvmodule.h"
#include "fromfilemodule.h"
#include "osutil.h"

/* place where the *.hm *.csv and compilers are stored */
std::string dataDir;
/* place where the *.hmc are created */
std::string userDir;

int main(int argc, char *argv[])
{
    std::cout<<std::endl;
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("HexaMonkey");
    QCoreApplication::setApplicationName("HexaMonkey");

#if defined(PLATFORM_WIN32)
    dataDir = ".\\";
    userDir = ".\\";
#elif defined(PLATFORM_LINUX)
    /* XXX: use autotools ? */
    dataDir = "/usr/local/share/hexamonkey/";
    userDir = getenv("HOME");
    userDir += "/.hexamonkey/";
#else
    dataDir = "./";
    userDir = "./";
#endif

    QDir dir = QDir::root();
    dir.mkdir(QString(userDir.c_str()));

    ModuleLoader moduleLoader;

    moduleLoader.addModule("bestd", new StandardModule(true));
    moduleLoader.addModule("lestd", new StandardModule(false));
    moduleLoader.addModule("ebml",  new EbmlModule);
    moduleLoader.addModule("mkv",   new MkvModule(dataDir));
    moduleLoader.addModule("hmc",   new HmcModule(dataDir));

    ProgramLoader programLoader(static_cast<const HmcModule&>(moduleLoader.getModule("hmc")),
                                dataDir, userDir);

#ifdef __MINGW32__
    moduleLoader.addFolder(dataDir + ".\\scripts\\", programLoader);    
    moduleLoader.addFolder(dataDir + "..\\scripts\\", programLoader);
#else
    moduleLoader.addFolder(dataDir + "./scripts/", programLoader);
#endif

    MainWindow window(moduleLoader, programLoader);

    window.setWindowTitle("HexaMonkey");
    window.setWindowIcon(QIcon(":/logo.svg"));
    window.showMaximized();

    for(int i = 1; i < argc; ++i)
    {
        window.openFile(argv[i]);
    }
    
    return a.exec();
}
