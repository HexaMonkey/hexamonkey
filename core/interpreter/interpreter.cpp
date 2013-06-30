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

#include "interpreter.h"
#include <sstream>
#ifdef USE_QT
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#else
#include <cstdlib>
#include <cstdio>
#endif

#include "defaulttypes.h"
#include "model.h"

#include "unused.h"
#include "hmcmodule.h"
#include "variable.h"
#include "variabledescriptor.h"
#include "scope.h"
#include "functionscope.h"
#include "objecttypetemplate.h"
#include "parser.h"


Interpreter::Interpreter(const HmcModule &module) : _module(module)
{
    UNUSED(hmcElemNames);
}

Program Interpreter::loadFromString(const std::string &exp) const
{
    std::ofstream f("temp.hm");
    f<<exp<<std::endl;
    f.close();
    return loadFromHM("temp.hm", expression);
}

Program Interpreter::loadFromHM(const std::string &path, int mode) const
{
    const std::string outputName = path+"c";

#ifdef USE_QT
    QProcess process;
    QString base;
    if(mode == file)
         base = "hexacompiler";
    else
         base = "expcompiler";

    #ifdef __MINGW32__
    QString extension = ".exe";
    #else
    QString extension = "";
    #endif
    QFileInfo compiler;



    compiler.setFile(QString("./%1%2").arg(base).arg(extension));
    if(!compiler.exists())
    {
        compiler.setFile(QString("../compiler/%1%2").arg(base).arg(extension));
    }
    if(!compiler.exists())
    {
        std::cerr<<"Compiler not found"<<std::endl;
        return Program();
    }


    QStringList arguments;
    QFileInfo inputFile(path.c_str());
    QString arg1 = inputFile.absoluteFilePath();
    QString arg2 = QString("%1c").arg(arg1);
    arguments<<arg1<<arg2;
    std::cout<<"Program "<<compiler.absoluteFilePath().toStdString()<<std::endl;
    std::cout<<"Arguments "<<arg1.toStdString()<<" "<<arg2.toStdString()<<std::endl;
    process.start(compiler.absoluteFilePath(), arguments);
    process.waitForFinished();
    std::string output =  QString(process.readAllStandardOutput()).toStdString();
    std::string error  = QString(process.readAllStandardError()).toStdString();
    std::cout<<QString(process.readAllStandardOutput()).toStdString()<<std::endl;

    if(!output.empty())
    {
        std::cout<<"Compiler output : "<<output<<std::endl;
    }

    if(!error.empty())
    {
        std::cerr<<"Compiler error : "<<error<<std::endl;
        return Program();
    }
#else
    std::cout<<"don't use qt"<<std::endl;
    std::stringstream commandStream;
#ifndef linux
    commandStream<<"..\\compiler\\";
#else
    commandStream<<"../compiler/"
#endif

    if(mode==file)
        commandStream<<"hexacompiler";
    else
        commandStream<<"expcompiler";

    commandStream<<" "<<path<<" "<<outputName;
    const char* command = commandStream.str().c_str();

#ifndef linux
    _pclose(_popen(command, "r"));
#else
    pclose(popen(command, "r"));
#endif
#endif
    return loadFromHMC(outputName);
}

Program Interpreter::loadFromHMC(const std::string &path) const
{
    auto pmemory = std::make_shared<Program::Memory>();
    Program::Memory& memory = *pmemory;

    File& file = memory.file();
    file.setPath(path);

    Object& fileObject = memory.setFileObject(_module.handle(defaultTypes::file(), file));
    fileObject.explore(-1);

    if(fileObject.numberOfChildren() >= 2)
    {
        return Program(*fileObject.access(1), pmemory);
    }
    else
    {
        std::cout << "failure" << std::endl;
        return Program();
    }
}

Program Interpreter::loadFromFile(const std::string &path) const
{
    std::string hmPath  = path+".hm";
    std::string hmcPath = path+".hmc";

#ifdef USE_QT

    QFileInfo hmInfo(hmPath.c_str());
    QFileInfo hmcInfo(hmcPath.c_str());
    if(!hmInfo.exists())
    {
        if(!hmcInfo.exists())
        {
            std::cerr<<"Description file not found: "<<hmPath<<std::endl;
            return Program();
        }
        std::cout<<"Load existing description file : "<<hmcPath<<std::endl;
        return loadFromHMC(hmcPath);
    }
    else
    {
        if(!hmcInfo.exists() || hmcInfo.lastModified() < hmInfo.lastModified())
        {
            std::cout<<"Compile description file : "<<hmPath<<std::endl;
            return loadFromHM(hmPath, Interpreter::file);
        }
        std::cout<<"Load existing description file : "<<hmcPath<<std::endl;
        return loadFromHMC(hmcPath);
    }
#else
    if(fileExists(hmPath))
        return loadFromHM(hmPath, Interpreter::file);
    else if(fileExists(hmcPath))
        return loadFromHMC(hmcPath);
    else
        return false;
#endif
}
