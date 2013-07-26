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

#include "programloader.h"
#include <sstream>
#ifdef USE_QT
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#else
#include <cstdlib>
#include <cstdio>
#include "fileutil.h"
#endif

#include "defaulttypes.h"
#include "model.h"

#include "unused.h"
#include "hmcmodule.h"
#include "variable.h"
#include "variablepath.h"
#include "scope.h"
#include "functionscope.h"
#include "objecttypetemplate.h"
#include "parser.h"


ProgramLoader::ProgramLoader(const HmcModule &module, std::string &dataDir, std::string &userDir)
                            : _module(module), _dataDir(dataDir), _userDir(userDir)
                        
{
    UNUSED(hmcElemNames);
}

Program ProgramLoader::fromString(const std::string &exp) const
{
    std::ofstream f("temp.hm");
    f<<exp<<std::endl;
    f.close();
    return fromHM("temp.hm", expression);
}

Program ProgramLoader::fromHM(const std::string &path) const
{
    return fromHM(path, file);
}

Program ProgramLoader::fromHM(const std::string &path, int mode) const
{
    size_t pos;
    pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return Program();
    }
    const std::string outputName = _userDir + path.substr(pos) + "c";

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



    compiler.setFile(QString((_dataDir + "./%1%2").c_str()).arg(base).arg(extension));
    if(!compiler.exists())
    {
        std::cerr<<"Compiler not found"<<std::endl;
        return Program();
    }


    QStringList arguments;
    QFileInfo inputFile(path.c_str());
    QString arg1 = inputFile.absoluteFilePath();
    QString arg2 = QString(outputName.c_str());
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

#ifdef PLATFORM_LINUX
    pclose(popen(command, "r"));
#else
    _pclose(_popen(command, "r"));
#endif

#endif
    return fromHMC(outputName);
}

Program ProgramLoader::fromHMC(const std::string &path) const
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

Program ProgramLoader::fromFile(const std::string &path) const
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
        return fromHMC(hmcPath);
    }
    else
    {
        if(!hmcInfo.exists() || hmcInfo.lastModified() < hmInfo.lastModified())
        {
            std::cout<<"Compile description file : "<<hmPath<<std::endl;
            return fromHM(hmPath, ProgramLoader::file);
        }
        std::cout<<"Load existing description file : "<<hmcPath<<std::endl;
        return fromHMC(hmcPath);
    }
#else
    if(fileExists(hmPath))
        return fromHM(hmPath, ProgramLoader::file);
    else if(fileExists(hmcPath))
        return fromHMC(hmcPath);

    return Program();
#endif
}
