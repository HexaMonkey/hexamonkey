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

#include "core/util/fileutil.h"
#include "core/util/iterutil.h"

#include <fstream>
#include <dirent.h>


bool fileExists(const std::string& path)
{
    if(path.empty())
        return false;

    std::ifstream file(path);
    return file.is_open();
}


std::string getFile(const std::vector<std::string>& directories, const std::string& fileName)
{
    for(const std::string& directory : reverse(directories))
    {
        std::string path = directory+fileName;
        if(fileExists(path))
        {
            return path;
        }
    }
    return "";
}

void getDirContent(const std::string& path, std::vector<std::string>& content)
{
    DIR *dir = NULL;
    struct dirent *drnt = NULL;
    dir=opendir(path.c_str());
    if(dir)
    {
        while((drnt = readdir(dir)))
        {
            content.push_back(drnt->d_name);
        }
        closedir(dir);
    }
}
