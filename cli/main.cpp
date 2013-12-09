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

#include "core/interpreter/fromfilemodule.h"
#include "core/moduleloader.h"
#include "core/interpreter/programloader.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/default/defaulttypes.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/modules/mkv/mkvmodule.h"
#include "core/modules/standard/standardmodule.h"
#include "core/util/fileutil.h"
#include "core/util/osutil.h"

#if defined(PLATFORM_WIN32)
    #include <windef.h>
    #include <winbase.h>
#endif

int main(int argc, char *argv[])
{



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
    std::string installDir = "/usr/local/share/hexamonkey/";
    std::string userDir = std::string(getenv("HOME"))+"/.hexamonkey/";
    std::vector<std::string> modelsDirs = {installDir, "../models/"};
    std::vector<std::string> scriptsDirs = {installDir+"scripts/", userDir, "../scripts/"};
    std::vector<std::string> compilerDirs = {installDir, "../compiler/"};
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

    ProgramLoader programLoader(static_cast<const HmcModule&>(moduleLoader.getModule("hmc")), compilerDirs, userDir);

    moduleLoader.addDirectories(scriptsDirs, programLoader);


    File file;
    if (argc < 2)
    {
        std::cerr << "No input specified" << std::endl;
        return 1;
    }
    file.setPath(argv[1]);
    if (!file.good())
    {
        std::cerr << "File not found" <<std::endl;
    }
    else
    {
        const Module& module = moduleLoader.getModule(file);
        std::vector<std::string> prout = module.getFunctionParameterNames("prout");
        std::vector<Object*> objs;
        objs.push_back(module.handle(defaultTypes::file, file));
        Object*child = nullptr;
        for (int i = 0; i < argc-2; ++i)
        {
            objs[i]->explore(1);
            char* child_id = argv[i+2];
            child = objs[i]->lookUp(child_id);
            if (child == nullptr) {
                child = objs[i]->access(std::stoi(child_id));
            }
            if(child) {
                objs.push_back(child);
            } else {
                std::cerr << "object not found" << std::endl;
                return 1;
            }
        }
        objs[objs.size()-1]->explore(-1);
        std::cout << *(objs[objs.size()-1]) << std::endl;
    }
    return 0;
}
