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
#include <cstdlib>
#include <cstdio>

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
#include "core/error/errormanager.h"

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
    ErrorManager::getInstance()->notify("Error: unsuported operating system");
    return Program();
#endif



    std::string compiler;
    if(mode == file)
        compiler = _fileCompiler;
    else
        compiler = _expCompiler;

    if(!fileExists(path) || compiler.compare("") == 0)
    {
        ErrorManager::getInstance()->notify("Compiler not found");
        return Program();
    }

    const std::vector<std::string> arguments = {path, outputPath};

    std::cerr<<"Program "<<compiler<<std::endl;
    std::cerr<<"Arguments "<<path<<" "<<outputPath<<std::endl;

    bool success = executeCommand(compiler, arguments);

    if(success)
    {
        return fromHMC(outputPath);
    }
    else
    {
        return Program();
    }
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
        ErrorManager* em = ErrorManager::getInstance();
        em->errorMessage << "Script could not be loaded : "<< path << std::endl;
        em->notify();
        return Program();
    }
}

Program ProgramLoader::fromFile(const std::string &path) const
{
    std::string hmPath  = path+".hm";
    std::string hmcPath = path+".hmc";

    if(fileExists(hmPath))
    {
        if(fileExists(hmcPath)) {
            long long hmcLastModified = lastModified(hmcPath);
            long long hmLastModified  = lastModified(hmPath);
            if(hmLastModified != -1 && hmLastModified != -1 && hmLastModified < hmcLastModified)
            {
                std::cerr<<"Load existing description file : "<<hmcPath<<std::endl;
                return fromHMC(hmcPath);
            }
        }
        std::cerr<<"Compile description file : "<<hmPath<<std::endl;
        return fromHM(hmPath, ProgramLoader::file);
    }
    else if(fileExists(hmcPath))
    {
        std::cerr<<"Load existing description file : "<<hmcPath<<std::endl;
        return fromHMC(hmcPath);
    }
    else
    {
        ErrorManager* em = ErrorManager::getInstance();
        em->errorMessage <<"Description file not found: "<<hmPath;
        em->notify();
    }

    return Program();
}

#ifdef PLATFORM_LINUX
#define __POPEN popen
#define __PCLOSE pclose
#else
#define __POPEN _popen
#define __PCLOSE _pclose
#endif

bool ProgramLoader::executeCommand(const std::string &program, const std::vector<std::string> &arguments) const
{
    std::stringstream commandStream;

    commandStream<<program;
    for(const std::string& argument : arguments)
    {
        commandStream<<" "<<argument;
    }

    std::string s = commandStream.str();


    FILE *f = __POPEN(s.c_str(), "r");
    if(f == nullptr)
    {
        return false;
    }

    while (!feof(f)) {
        char buf[512];
        int r = fread(buf, 1, 512, f);
        std::cerr.write(buf, r);
    }

    int status = __PCLOSE(f);
    std::cerr<<"Compiler status: "<<status<<std::endl;

    return (status == EXIT_SUCCESS);
}

// TODO
long long ProgramLoader::lastModified(const std::string &/*file*/) const
{
    return -1LL;
}
