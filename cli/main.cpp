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

#include "core/error/errorobserver.h"
#include "core/error/errormanager.h"
#include "core/interpreter/fromfilemodule.h"
#include "core/moduleloader.h"
#include "core/interpreter/programloader.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/default/defaulttypes.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/modules/mkv/mkvmodule.h"
#include "core/modules/standard/standardmodule.h"
#include "core/modules/stream/streammodule.h"
#include "core/util/fileutil.h"
#include "core/util/osutil.h"

#if defined(PLATFORM_WIN32)
    #include <windef.h>
    #include <winbase.h>
#endif

enum DisplayType {
    fileType,
    subtree,
    info,
    value,
    numberOfChildren,
    size,
    binary
};
#define DISPLAY_TYPE_DEFAULT DisplayType::subtree

struct CLIOptions {
    std::string filePath;
    std::vector<std::string> leafs;
    DisplayType displayType;
    int maxDepth;
    CLIOptions() : filePath(), leafs(), displayType(DISPLAY_TYPE_DEFAULT),
                   maxDepth(-1) {};
};
typedef struct CLIOptions CLIOptions;

class ErrorCLIObserver : public ErrorObserver {
public:
    virtual void update(std::string errorRaised, LogLevel level) override;
};

void ErrorCLIObserver::update(std::string errorRaised, LogLevel /*level*/) {
    std::cerr << "[ERROR]" << errorRaised << std::endl;
}

void print_help()
{
    std::cout << "\
Usage: hexamonkey-cli FILE [OPTIONS] [[LEAF] ...]\n\
       hexamonkey-cli [--help]\n\
The hexamonkey CLI analyses a binary file and extracts information from it. \
It will go down in the file given a list of leafs (which can either be a field\
 name or an index). Add '+' at the beginning of a leaf name to force it as an\
index.\n\
\n\
Example: hexamonkey-cli test.mp4 1 type\n\
This will look for a field named 'type' in the second item of the mp4 file.\n\
\n\
Options:\n\
  -t, --display-type : What to display. It can be :\n\
     * fileType : file type detected using extension or magic number (leafs \
are ignored),\n\
     * subtree : tree describing the last leaf reached,\n\
     * info : info on the last leaf reached,\n\
     * value : value of the last leaf reached, \n\
     * numberOfChildren : number of children of the last leaf reached,\n\
     * size : size of the last leaf reached (in bits) \n\
     * binary : raw data of the last leaf reached (use with caution) \n\
  -d, --max-depth : how deep in the subtree should we go,\n\
                    ignored if type is not subtree\n\
                    -1 (default) will go as deep as it gets" << std::endl;
}


bool parseArgs(const int argc, const char* const argv[], CLIOptions& options)
{
    std::list<std::string> optStr;
    for(int i = 1; i < argc; ++i)
        optStr.push_back(argv[i]);

    if (optStr.empty())
    {
        std::cerr << "No input specified" << std::endl;
        return false;
    }
    if(optStr.front() == "--help")
        return false;
    options.filePath = optStr.front();
    optStr.pop_front();

    bool moreOptions = true;
    while(!optStr.empty() && moreOptions)
    {
        std::string flag(optStr.front());
        if(flag == "--display-type" || flag == "-t")
        {
            optStr.pop_front();
            if(optStr.empty())
                return false;

            std::string typeStr(optStr.front());
            optStr.pop_front();
            if(typeStr == "fileType")
                options.displayType = DisplayType::fileType;
            else if(typeStr == "subtree")
                options.displayType = DisplayType::subtree;
            else if(typeStr == "info")
                options.displayType = DisplayType::info;
            else if(typeStr == "value")
                options.displayType = DisplayType::value;
            else if(typeStr == "numberOfChildren")
                options.displayType = DisplayType::numberOfChildren;
            else if(typeStr == "size")
                options.displayType = DisplayType::size;
            else if(typeStr == "binary")
                options.displayType = DisplayType::binary;
            else
                return false;
        } else if(flag == "--max-depth" || flag == "-d")
        {
            optStr.pop_front();
            if(optStr.empty())
                return false;

            std::stringstream mdStream(optStr.front());
            mdStream >> options.maxDepth;
            optStr.pop_front();
        } else
        { moreOptions = false; }
    }

    for(auto& leaf : optStr)
        options.leafs.push_back(leaf);

    return true;
}

int main(int argc, char *argv[])
{
    ErrorCLIObserver errorCliOsberver;
    ErrorManager::getInstance()->attach(&errorCliOsberver);

    CLIOptions options;
    if(!parseArgs(argc, argv, options))
    {
        print_help();
        return 1;
    }

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

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
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
    moduleLoader.addModule("stream",new StreamModule());

    ProgramLoader programLoader(static_cast<const HmcModule&>(moduleLoader.getModule("hmc")), compilerDirs, userDir);

    moduleLoader.setDirectories(scriptsDirs, programLoader);


    RealFile file;
    file.setPath(argv[1]);
    if (!file.good())
    {
        std::cerr << "File not found" <<std::endl;
    }
    else
    {
        const Module& module = moduleLoader.getModule(file);

        std::vector<Object*> objs;
        objs.push_back(module.handle(defaultTypes::file, file));


        Object*child = nullptr;
        for (auto& leaf : options.leafs)
        {
            const char* child_id = leaf.c_str();
            if(child_id[0] == '+')
            {
                child = objs[0]->access(std::stoi(child_id+1), true);
            } else {
                child = objs[0]->lookUp(child_id, true);
                if (child == nullptr) {
                    child = objs[0]->access(std::stoi(child_id), true);
                }
            }
            if(child) {
                objs.insert(objs.begin(), child);
            } else {
                std::cerr << "Object not found" << std::endl;
                return 1;
            }
        }
        objs[0]->explore(options.maxDepth);
        switch(options.displayType)
        {
            case fileType:
                std::cout << objs[objs.size()-1]->type() << std::endl;
                break;

            case info:
                std::cout << objs[0]->info() << std::endl;
                break;

            case value:
                std::cout << objs[0]->value() << std::endl;
                break;

            case subtree:
                std::cout << *(objs[0]) << std::endl;
                break;

            case numberOfChildren:
                std::cout << objs[0]->numberOfChildren() << std::endl;
                break;

            case size:
                std::cout << objs[0]->size() << std::endl;
                break;

            case binary:
                objs[0]->dump(std::cout);
                break;

            default:
                break;
        }
    }
    return 0;
}
