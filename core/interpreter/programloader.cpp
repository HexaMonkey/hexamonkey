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

#include "compiler/model.h"
#include "core/modules/default/defaulttypes.h"
#include "core/objecttypetemplate.h"
#include "core/parser.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/interpreter/programloader.h"
#include "core/interpreter/variable.h"
#include "core/interpreter/variablepath.h"
#include "core/interpreter/scope/scope.h"
#include "core/interpreter/scope/functionscope.h"
#include "core/util/unused.h"
#include "core/util/osutil.h"
#include "core/util/fileutil.h"

ProgramLoader::ProgramLoader(const HmcModule &module, const std::vector<std::string> &compilerDirs, const std::string userDir)
    : _module(module),
#ifdef PLATFORM_WIN32
      _fileCompiler(getFile(compilerDirs, "hexacompiler.exe")),
      _expCompiler (getFile(compilerDirs, "expcompiler.exe")),
#else
      _fileCompiler(getFile(compilerDirs, "hexacompiler")),
      _expCompiler (getFile(compilerDirs, "expcompiler")),
#endif
      _userDir(userDir)
                        
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
#if defined(PLATFORM_WIN32)
    const std::string outputPath = path+"c";

#elif defined(PLATFORM_LINUX)
    size_t pos;
    pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        pos = 0;
    }
    const std::string outputPath = _userDir + path.substr(pos) + "c";

#else
    std::cerr<<"Error: unsuported operating system"<<std::endl;
    return 0;
#endif


 
    std::string compiler;
    if(mode == file)
        compiler = _fileCompiler;
    else
        compiler = _expCompiler;

    if(!fileExists(path) || compiler.compare("") == 0)
    {
        std::cerr<<"Compiler not found"<<std::endl;
        return Program();
    }

    std::cerr<<"Program "<<compiler<<std::endl;
    std::cerr<<"Arguments "<<path<<" "<<outputPath<<std::endl;

#ifdef USE_QT
    QProcess process;

    QStringList arguments;
    arguments<<QString(path.c_str())<<QString(outputPath.c_str());

    process.start(QString(compiler.c_str()), arguments);
    process.waitForFinished();
    std::string output =  QString(process.readAllStandardOutput()).toStdString();
    std::string error  = QString(process.readAllStandardError()).toStdString();

    if(!output.empty())
    {
        std::cerr<<"Compiler output : "<<output<<std::endl;
    }

    if(!error.empty())
    {
        std::cerr<<"Compiler error : "<<error<<std::endl;
        return Program();
    }
#else
    std::stringstream commandStream;

    commandStream<<compiler<<" "<<path<<" "<<outputPath;
    const char* command = commandStream.str().c_str();

#ifdef PLATFORM_LINUX
    pclose(popen(command, "r"));
#else
    _pclose(_popen(command, "r"));
#endif

#endif
    return fromHMC(outputPath);
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
        std::cerr << "Script could not be loaded : "<< path << std::endl;
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
        std::cerr<<"Load existing description file : "<<hmcPath<<std::endl;
        return fromHMC(hmcPath);
    }
    else
    {
        if(!hmcInfo.exists() || hmcInfo.lastModified() < hmInfo.lastModified())
        {
            std::cerr<<"Compile description file : "<<hmPath<<std::endl;
            return fromHM(hmPath, ProgramLoader::file);
        }
        std::cerr<<"Load existing description file : "<<hmcPath<<std::endl;
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
